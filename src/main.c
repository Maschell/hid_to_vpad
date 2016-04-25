#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/syshid_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "common/common.h"
#include "game/rpx_rpl_table.h"
#include "game/memory_area_table.h"
#include "start.h"
#include "patcher/function_hooks.h"
#include "kernel/kernel_functions.h"
#include "system/exception_handler.h"
#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"
#include "controller_patcher/controller_patcher.h"

typedef union u_serv_ip
{
    uint8_t  digit[4];
    uint32_t full;
} u_serv_ip;

#define PRINT_TEXT2(x, y, ...) { snprintf(msg, 80, __VA_ARGS__); OSScreenPutFontEx(0, x, y, msg); OSScreenPutFontEx(1, x, y, msg); }

/* Entry point */
int Menu_Main(void)
{
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    //! do OS (for acquire) and sockets first so we got logging
    InitOSFunctionPointers();
    InitSocketFunctionPointers();
    InitGX2FunctionPointers();
    InitSysFunctionPointers();

    log_init("192.168.0.181");

    SetupKernelCallback();
    log_printf("Started %s\n", cosAppXmlInfoStruct.rpx_name);

    //!*******************************************************************
    //!                        Initialize HID Config                     *
    //!*******************************************************************
    init_config_controller();

    PatchMethodHooks();
    if(strlen(cosAppXmlInfoStruct.rpx_name) > 0 && strcasecmp("ffl_app.rpx", cosAppXmlInfoStruct.rpx_name) != 0)
    {
        return EXIT_RELAUNCH_ON_LOAD;
    }

    if(strlen(cosAppXmlInfoStruct.rpx_name) <= 0){ // First boot back to SysMenu
        log_printf("Startssed %s\n", cosAppXmlInfoStruct.rpx_name);
        SYSLaunchMenu();
        return EXIT_RELAUNCH_ON_LOAD;
    }
    log_printf("Stafdsf %s\n", cosAppXmlInfoStruct.rpx_name);
    RestoreInstructions();

    deinit_config_controller();

    log_deinit();


    return EXIT_SUCCESS;
}

