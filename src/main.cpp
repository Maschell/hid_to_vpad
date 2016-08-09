#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "main.h"
#include "version.h"
#include "common/common.h"

#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/syshid_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "controller_patcher/controller_patcher.h"
#include "controller_patcher/config_reader.h"
#include "controller_patcher/cp_retain_vars.h"
#include "utils/function_patcher.h"
#include "patcher/hid_controller_patcher.h"
#include "kernel/kernel_functions.h"
#include "video/CursorDrawer.h"
#include "utils/logger.h"

#define PRINT_TEXT1(x, y, str) { OSScreenPutFontEx(1, x, y, str); OSScreenPutFontEx(0, x, y, str); }
#define PRINT_TEXT2(x, y, _fmt, ...) { __os_snprintf(msg, 80, _fmt, __VA_ARGS__); OSScreenPutFontEx(0, x, y, msg);OSScreenPutFontEx(1, x, y, msg); }

/* Entry point */
extern "C" int Menu_Main(void)
{
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitOSFunctionPointers();
    InitSocketFunctionPointers();
    InitGX2FunctionPointers();
    InitSysFunctionPointers();
    InitVPadFunctionPointers();

    SetupKernelCallback();

    log_init("192.168.0.181");

    //Reset everything when were going back to the Mii Maker
    if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) == 0){
        log_print("Returing to the Homebrew Launcher!\n");
        deInit();
        return EXIT_SUCCESS;
    }

    //Otherwise apply the patches!!!
    log_printf("HID to VPAD %s - %s %s - by Maschell\n\n",APP_VERION,__DATE__,__TIME__);

    //!*******************************************************************
    //!                        Initialize HID Config                     *
    //!*******************************************************************
    log_print("Initializing the controller data\n");
    init_config_controller();
    log_print("Initializing the data for button remapping\n");
    init_button_remapping();
    if(gConfig_done == HID_INIT_DONE){
        log_print("Reading config files from SD Card\n");
        ConfigReader::getInstance(); //doing the magic automatically
        ConfigReader::destroyInstance();
        log_print("Done with reading config files from SD Card\n");
        gConfig_done = HID_SDCARD_READ;
    }

    //!*******************************************************************
    //!                        Patching functions                        *
    //!*******************************************************************
    log_print("Patching functions\n");
    ApplyPatches();

    if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0)
    {
        return EXIT_RELAUNCH_ON_LOAD;
    }

    if(strlen(cosAppXmlInfoStruct.rpx_name) <= 0){ // First boot back to SysMenu
        SplashScreen(15);
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }

    deInit();
    return EXIT_SUCCESS;
}
void ApplyPatches(){
    PatchInvidualMethodHooks(method_hooks_hid_controller,   method_hooks_size_hid_controller,   method_calls_hid_controller);
}
void deInit(){
    CursorDrawer::destroyInstance();
    RestoreInvidualInstructions(method_hooks_hid_controller,method_hooks_size_hid_controller);
    deinit_config_controller();
    log_deinit();
}

void SplashScreen(int time){
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

    char* msg = (char*) malloc(80);
    PRINT_TEXT2(0,0, "HID to VPAD %s - %s %s - by Maschell",APP_VERION,__DATE__,__TIME__);

    int readFiles = ConfigReader::getNumberOfLoadedFiles();
    int i = 2;

    PRINT_TEXT1(0,i, " - Reading SD Card."); i++;
    if(readFiles > 0){
        PRINT_TEXT2(0,i, " - Loaded %d valid config files",readFiles); i++;

    }else{
        PRINT_TEXT1(0,i, " - No valid config files found."); i++;
        PRINT_TEXT1(0,i, " - Starting with default values"); i++;
    }
    PRINT_TEXT1(0,i,   " - Applied functions patches"); i++;i++;
    PRINT_TEXT2(0,i, "Wait %d seconds or press A to return to the system menu",time); i++;

    PRINT_TEXT1(0,9, "Support for new gamepads can be added through config files"); i++;

    PRINT_TEXT1(0,11, "Download new configs here: ");
    PRINT_TEXT1(0,12, "https://github.com/Maschell/controller_patcher_configs");
    //13
    PRINT_TEXT1(0,14, "Support:");
    PRINT_TEXT1(0,15, "github     : https://github.com/Maschell/hid_to_vpad");
    PRINT_TEXT1(0,16, "             https://github.com/Maschell/controller_patcher");
    PRINT_TEXT1(0,17, "gbatemp.net: http://gbatemp.net/threads/hid-to-vpad.424127/");

    OSScreenFlipBuffersEx(0);
    OSScreenFlipBuffersEx(1);

    VPADData vpad_data;
    int error;

    int tickswait = time * 1000*1000;
    int times = 1000;
    int sleepingtime = tickswait / 1000;
    i=0;
    while(i<times){
        VPADRead(0, &vpad_data, 1, &error);
        if(vpad_data.btns_h & VPAD_BUTTON_A){
            i = times;
        }
        i++;
        usleep(sleepingtime);
    }
    free(msg);
}
