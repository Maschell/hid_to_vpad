#include "main.h"

#include "version.h"

#include "controller_patcher/ControllerPatcher.hpp"

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/syshid_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/proc_ui_functions.h"

#include "utils/logger.h"
#include "utils/utils.h"
#include "utils/function_patcher.h"

#include "patcher/hid_controller_function_patcher.hpp"

#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"

#include "system/memory.h"
#include "kernel/kernel_functions.h"
#include "common/common.h"

#include "language/gettext.h"

#include "video/CursorDrawer.h"

#include "Application.h"
#include "settings/CSettings.h"

#include "utils/StringTools.h"

/* Entry point */
extern "C" s32 Menu_Main(void){
    if(gAppStatus == 2){
        //"No, we don't want to patch stuff again.");
        return EXIT_RELAUNCH_ON_LOAD;
    }
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitOSFunctionPointers();
    InitVPadFunctionPointers();
    InitProcUIFunctionPointers();

    if(OSGetTitleID()  == 0x00050000101a5f00L || // Shantae and the Pirate's Curse USA (reason: crashes when pressing HOME, Pro Controller not recognized)
        OSGetTitleID() == 0x00050000101F7F00L || // Shantae and the Pirate's Curse JAP (シャンティ -海賊の呪い-) (reason: crashes when pressing HOME, Pro Controller not recognized)
        OSGetTitleID() == 0x00050000101a9500L){  // Shantae and the Pirate's Curse EUR (reason: crashes when pressing HOME, Pro Controller not recognized)
        bool res = SplashScreen(5, std::string("This game is not fully supported. Press X to keep patches applied.").c_str(),0,VPAD_BUTTON_X);
        if(res){
            SplashScreen(0, std::string("Keeping patches applied. Pressing Home results in a crash.").c_str(),1,0);
            SplashScreen(5, std::string("Pro Controller emulation is not supported.").c_str(),2,0);
        }else{
            SplashScreen(3, std::string("Unloading patches. The game should work normally!").c_str(),1,0);
            RestorePatches();
            return EXIT_RELAUNCH_ON_LOAD;
        }
    }

    InitFSFunctionPointers();
    InitSocketFunctionPointers();
    InitGX2FunctionPointers();
    InitSysFunctionPointers();

    InitPadScoreFunctionPointers();
    InitAXFunctionPointers();

    SetupKernelCallback();

    log_init("192.168.0.181");
    log_printf("HID to VPAD %s - %s %s - by Maschell\n\n",APP_VERION,__DATE__,__TIME__);

    //!*******************************************************************
    //!                        Initialize HID Config                     *
    //!*******************************************************************
    log_printf("Menu_Main (line %d): Initializing the controller data\n",__LINE__);
    bool res = ControllerPatcher::Init();
    if(!res){
        SplashScreen(5, std::string("Error. The app starts in 5 seconds without patches.").c_str(),0,0);
        RestorePatches();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    ControllerPatcher::enableControllerMapping();
    ControllerPatcher::startNetworkServer();

     //!*******************************************************************
    //!                        Patching functions                        *
    //!*******************************************************************
    log_printf("Menu_Main(line %d): Patching functions\n",__LINE__);
    ApplyPatches();


    s32 result = 0;
    if(isInMiiMakerHBL()){
        //!*******************************************************************
        //!                    Initialize heap memory                        *
        //!*******************************************************************
        log_printf("Menu_Main (line %d): Initialize memory management\n",__LINE__);
        memoryInitialize();

        log_printf("Menu_Main (line %d): Mount SD partition\n",__LINE__);
        mount_sd_fat("sd");
        log_printf("Menu_Main (line %d): Start main application\n",__LINE__);
        result = Application::instance()->exec();
        log_printf("Menu_Main (line %d): Main application stopped result: %d\n",__LINE__,result);
        Application::destroyInstance();
        bool rumble = CSettings::instance()->getValueAsBool(CSettings::RumbleActivated);
        log_printf("Menu_Main (line %d): Setting rumble to: %d\n",__LINE__,rumble);
        ControllerPatcher::setRumbleActivated(rumble);
        bool networkController = CSettings::instance()->getValueAsBool(CSettings::NetworkControllerActivated);
        ControllerPatcher::setNetworkControllerActivated(networkController);
        log_printf("Menu_Main (line %d): Unmount SD\n",__LINE__);
        unmount_sd_fat("sd");
        log_printf("Menu_Main (line %d): Release memory\n",__LINE__);
        memoryRelease();
        ControllerPatcher::destroyConfigHelper();
        CSettings::destroyInstance();
    }




    if(!isInMiiMakerHBL()){
        ControllerPatcher::disableWiiUEnergySetting();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    if(result == APPLICATION_CLOSE_APPLY){
        log_printf("Menu_Main (line %d): Loading the system menu.\n",__LINE__);
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }
    log_printf("Menu_Main (line %d): Going back to the Homebrew Launcher\n",__LINE__);
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
s32 isInMiiMakerHBL(){
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


bool SplashScreen(s32 time,const char * message,u8 pos,u32 button){
    bool result = false;
    // Prepare screen
    s32 screen_buf0_size = 0;

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

    OSScreenPutFontEx(0, 0, pos, message);
    OSScreenPutFontEx(1, 0, pos, message);

    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    s32 tickswait = time * 1000*1000;
    s32 times = 1000;
    s32 sleepingtime = tickswait / 1000;
    s32 i=0;

    VPADData vpad_data;
    s32 error;
    while(i<times){
        VPADRead(0, &vpad_data, 1, &error);
        if(button > 0 && (vpad_data.btns_h & button)){
            result = true;
            i = times;
        }
        i++;
        os_usleep(sleepingtime);
    }
    return result;
}
