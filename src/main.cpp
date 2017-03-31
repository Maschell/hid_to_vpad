#include "Application.h"
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/syshid_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "common/common.h"

#include "main.h"
#include "version.h"
#include "common/common.h"

#include "controller_patcher/ControllerPatcher.hpp"
#include "controller_patcher/ConfigReader.hpp"
#include "controller_patcher/config/ConfigValues.hpp"
#include "controller_patcher/utils/CPRetainVars.hpp"
#include "controller_patcher/network/UDPServer.hpp"
#include "controller_patcher/network/TCPServer.hpp"
#include "utils/function_patcher.h"
#include "patcher/hid_controller_function_patcher.hpp"
#include "kernel/kernel_functions.h"
#include "kernel/syscalls.h"
#include "video/CursorDrawer.h"
#include "utils/logger.h"
#include "utils/StringTools.h"

#include "system/memory.h"

/* Entry point */
extern "C" int Menu_Main(void){
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitOSFunctionPointers();
    InitFSFunctionPointers();
    InitSocketFunctionPointers();
    InitGX2FunctionPointers();
    InitSysFunctionPointers();
    InitVPadFunctionPointers();
    InitPadScoreFunctionPointers();
    InitAXFunctionPointers();

    SetupKernelCallback();

    log_init("192.168.0.181");

    log_printf("HID to VPAD %s - %s %s - by Maschell\n\n",APP_VERION,__DATE__,__TIME__);

    //!*******************************************************************
    //!                        Initialize HID Config                     *
    //!*******************************************************************
    log_print("Initializing the controller data\n");
    bool res = ControllerPatcher::Init();
    if(!res){
        SplashScreen(5, "Error. The app starts in 5 seconds without patches.");
        RestorePatches();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    ControllerPatcher::enableControllerMapping();
    ControllerPatcher::startNetworkServer();

    int result = 0;
    if(isInMiiMakerHBL()){
        //!*******************************************************************
        //!                    Initialize heap memory                        *
        //!*******************************************************************
        log_print("Initialize memory management\n");
        memoryInitialize();

        log_printf("Mount SD partition\n");
        mount_sd_fat("sd");
        log_printf("Start main application\n");
        result = Application::instance()->exec();

        log_printf("Main application stopped result: %d\n",result);
        Application::destroyInstance();
        log_printf("Unmount SD\n");
        unmount_sd_fat("sd");
        memoryRelease();
        ControllerPatcher::destroyConfigHelper();
    }


    //!*******************************************************************
    //!                        Patching functions                        *
    //!*******************************************************************
    log_print("Patching functions\n");
    ApplyPatches();

    if(!isInMiiMakerHBL()){
        ControllerPatcher::disableWiiUEnergySetting();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    if(result == APPLICATION_CLOSE_APPLY){
        log_print("back to sysmenu\n");
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }
    log_print("back to hbl\n");
    ControllerPatcher::restoreWiiUEnergySetting();
    deInit();
    return EXIT_SUCCESS;
}

void ApplyPatches(){
    PatchInvidualMethodHooks(   method_hooks_hid_controller,    method_hooks_size_hid_controller,   method_calls_hid_controller);
}

void RestorePatches(){
    RestoreInvidualInstructions(method_hooks_hid_controller,    method_hooks_size_hid_controller);
}
void deInit(){
    CursorDrawer::destroyInstance();
    RestorePatches();
    ControllerPatcher::DeInit();
    ControllerPatcher::stopNetworkServer();
    log_deinit();
}
int isInMiiMakerHBL(){
    if (OSGetTitleID != 0 && (
            OSGetTitleID() == 0x000500101004A200 || // mii maker eur
            OSGetTitleID() == 0x000500101004A100 || // mii maker usa
            OSGetTitleID() == 0x000500101004A000 ||// mii maker jpn
            OSGetTitleID() == 0x0005000013374842))
        {
            return 1;
    }
    return 0;
}


void SplashScreen(int time,char * message){
    // Prepare screen
    int screen_buf0_size = 0;

    // Init screen and screen buffers
    OSScreenInit();
    screen_buf0_size = OSScreenGetBufferSizeEx(0);
    OSScreenSetBufferEx(0, (void *)0xF4000000);
    OSScreenSetBufferEx(1, (void *)(0xF4000000 + screen_buf0_size));

    OSScreenEnableEx(0, 1);
    OSScreenEnableEx(1, 1);

    // Clear screens
    OSScreenClearBufferEx(0, 0);
    OSScreenClearBufferEx(1, 0);

    // Flip buffers
    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    OSScreenPutFontEx(0, 0, 0, message);
    OSScreenPutFontEx(1, 0, 0, message);

    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    int tickswait = time * 1000*1000;
    int times = 1000;
    int sleepingtime = tickswait / 1000;
    int i=0;
    while(i<times){
        i++;
        usleep(sleepingtime);
    }
}
