/****************************************************************************
 * Copyright (C) 2016,2017 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <controller_patcher/ControllerPatcher.hpp>
#include <dynamic_libs/padscore_functions.h>
#include <video/CursorDrawer.h>
#include <utils/StringTools.h>
#include <utils/logger.h>

#include "hid_controller_function_patcher.hpp"
#include "common/retain_vars.h"

typedef void (* WPADSamplingCallback )( s32 chan );

DECL(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target){
    HID_Mouse_Data * mouse_data = ControllerPatcher::getMouseData();
    if(mouse_data !=  NULL){
        CursorDrawer::draw(mouse_data->X, mouse_data->Y);
    }
    real_GX2CopyColorBufferToScanBuffer(colorBuffer,scan_target);
}

DECL(void, __PPCExit, void){
    log_printf("__PPCExit\n");
    CursorDrawer::destroyInstance();

    ControllerPatcher::destroyConfigHelper();
    ControllerPatcher::stopNetworkServer();

    ControllerPatcher::resetCallbackData();

    real___PPCExit();
}

DECL(s32, VPADRead, s32 chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    s32 result = real_VPADRead(chan, buffer, buffer_size, error);
    //A keyboard only sends data when the state changes. We force it to call the sampling callback on each frame!
    ControllerPatcher::sampleKeyboardData();

    bool do_callback = (result > 0 && (buffer[0].btns_h & VPAD_BUTTON_TV));
    ControllerPatcher::handleCallbackData(do_callback);

    if(ControllerPatcher::areControllersConnected() && buffer_size > 0){
        ControllerPatcher::setRumble(UController_Type_Gamepad,!!VPADBASEGetMotorOnRemainingCount(0));

        if(ControllerPatcher::setControllerDataFromHID(buffer) == CONTROLLER_PATCHER_ERROR_NONE){

            if(buffer[0].btns_h & VPAD_BUTTON_HOME){
                //You can open the home menu this way, but not close it. Need a proper way to close it using the same button...
                //OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }

            if(error != NULL){
                *error = 0;
            }
            result = 1; // We want the WiiU to ignore everything else.
        }
    }

    if(ControllerPatcher::isButtonRemappingDone()){
        ControllerPatcher::buttonRemapping(buffer,result);
        //ControllerPatcher::printVPADButtons(buffer); //Leads to random crashes on app transitions.
    }

    return result;
}

DECL(s32, WPADProbe, s32 chan, u32 * result ){
   if( (chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
        if(result != NULL){
            *result = WPAD_EXT_PRO_CONTROLLER;
        }
        return 0;
    }

    return real_WPADProbe(chan,result);
}

DECL(wpad_connect_callback_t,WPADSetConnectCallback,s32 chan, wpad_connect_callback_t callback ){
    //log_printf("WPADSetConnectCallback chan %d %08X\n",chan,callback);

    ControllerPatcher::setWPADConnectCallback(chan,callback);

    if( (chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
            if(callback != NULL){
                callback(chan,0);
            }
    }
    return real_WPADSetConnectCallback(chan,callback);
}

DECL(wpad_extension_callback_t,WPADSetExtensionCallback,s32 chan, wpad_extension_callback_t callback ){
    //log_printf("WPADSetExtensionCallback chan %d %08X\n",chan,callback);

    ControllerPatcher::setKPADExtensionCallback(chan,callback);

    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
            if(callback != NULL){
                callback(chan,WPAD_EXT_PRO_CONTROLLER);
            }
    }
    return real_WPADSetExtensionCallback(chan,callback);
}

DECL(wpad_connect_callback_t,KPADSetConnectCallback,s32 chan, wpad_connect_callback_t callback ){
    //log_printf("KPADSetConnectCallback chan %d %08X\n",chan,callback);

    ControllerPatcher::setKPADConnectedCallback(chan,callback);

    if( (chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
            if(callback != NULL){
                callback(chan,0);
            }
    }
    return real_KPADSetConnectCallback(chan,callback);
}

DECL(u8, WPADGetBatteryLevel, s32 chan){
    u8 result = real_WPADGetBatteryLevel(chan);

   if( (chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
        result = 4; // Full battery
    }
    return result;
}

//In case a game relies on this...
DECL(u32, WPADGetDataFormat, s32 chan){
    //log_printf("WPADGetDataFormat chan: %d result: %d\n",chan,result);
    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
    (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
    (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
    (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
         return WPAD_FMT_PRO_CONTROLLER;
    }
    return real_WPADGetDataFormat(chan);
}

DECL(s32, WPADSetDataFormat, s32 chan, u32 fmt){
    s32 result = -1;
    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
    (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
    (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
    (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
        real_WPADSetDataFormat(chan,WPAD_FMT_PRO_CONTROLLER);
        result = 0;
    }
    result = real_WPADSetDataFormat(chan,fmt);
    return result;
}

DECL(void,WPADRead,s32 chan, WPADReadData *data ){
    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
            ControllerPatcher::setProControllerDataFromHID((void*)data,chan,PRO_CONTROLLER_MODE_WPADReadData);
    }else{
        real_WPADRead(chan,data);

    }
}

DECL(void,WPADControlMotor,s32 chan, u32 status ){
    if(chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)){
        ControllerPatcher::setRumble(UController_Type_Pro1,status);
    }else if(chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)){
        ControllerPatcher::setRumble(UController_Type_Pro2,status);
    }else if(chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)){
        ControllerPatcher::setRumble(UController_Type_Pro3,status);
    }else if(chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4)){
        ControllerPatcher::setRumble(UController_Type_Pro4,status);
    }
    real_WPADControlMotor(chan,status);
}

DECL(u32, ProcUIProcessMessages, u32 u){
    u32 res = real_ProcUIProcessMessages(u);
    if(res != gAppStatus){
        log_printf("App status changed from %d to %d \n",gAppStatus,res);
        gAppStatus = res;
    }

    return res;
}

hooks_magic_t method_hooks_hid_controller[] __attribute__((section(".data"))) = {
    MAKE_MAGIC(VPADRead,                            LIB_VPAD,       STATIC_FUNCTION),
    MAKE_MAGIC(GX2CopyColorBufferToScanBuffer,      LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(__PPCExit,                           LIB_CORE_INIT,  STATIC_FUNCTION),
    MAKE_MAGIC(WPADGetBatteryLevel,                 LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADSetConnectCallback,              LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADSetConnectCallback,              LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADSetExtensionCallback,            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADRead,                            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADGetDataFormat,                   LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADSetDataFormat,                   LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADControlMotor,                    LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADProbe,                           LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(ProcUIProcessMessages,               LIB_PROC_UI,   DYNAMIC_FUNCTION),
};

u32 method_hooks_size_hid_controller __attribute__((section(".data"))) = sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile u32 method_calls_hid_controller[sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

