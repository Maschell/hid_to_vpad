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
#include "controller_patcher/network/UDPServer.hpp"
#include "controller_patcher/network/TCPServer.hpp"
#include "controller_patcher/utils/CPRetainVars.hpp"
#include "hid_controller_function_patcher.hpp"
#include "dynamic_libs/padscore_functions.h"
#include "video/CursorDrawer.h"
#include "utils/StringTools.h"

#include "utils/logger.h"
#include "controller_patcher/ConfigReader.hpp"
#include "controller_patcher/config/ConfigValues.hpp"

DECL(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target){
    if(gHIDCurrentDevice & gHID_LIST_MOUSE && gHID_Mouse_Mode == HID_MOUSE_MODE_TOUCH) {
        HID_Mouse_Data * mouse_data = ControllerPatcher::getMouseData();
        if(mouse_data !=  NULL){
            CursorDrawer::draw(mouse_data->X, mouse_data->Y);
        }
    }
    real_GX2CopyColorBufferToScanBuffer(colorBuffer,scan_target);
}

DECL(void, __PPCExit, void){
    if(HID_DEBUG) log_printf("__PPCExit\n");
    CursorDrawer::destroyInstance();

    ControllerPatcher::destroyConfigHelper();
    ControllerPatcher::stopNetworkServer();

    memset(gWPADConnectCallback,0,sizeof(gWPADConnectCallback));
    memset(gKPADConnectCallback,0,sizeof(gKPADConnectCallback));
    memset(gExtensionCallback,0,sizeof(gExtensionCallback));
    gCallbackCooldown = 0;

    real___PPCExit();
}

DECL(s32, VPADRead, s32 chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    s32 result = real_VPADRead(chan, buffer, buffer_size, error);

    if(result > 0 && (buffer[0].btns_h & VPAD_BUTTON_TV) && gCallbackCooldown == 0){
        gCallbackCooldown = 0xFF;
        if(HID_DEBUG) log_printf("my_VPADRead(line %d): Pressed the TV button. Maybe we can call the callbacks.!\n",__LINE__);

        if(HID_DEBUG) log_printf("my_VPADRead(line %d): gExtensionCallback =  %08X %08X %08X %08X\n",__LINE__,gExtensionCallback[0],gExtensionCallback[1],gExtensionCallback[2],gExtensionCallback[3]);
        if(HID_DEBUG) log_printf("my_VPADRead(line %d): gWPADConnectCallback   =  %08X %08X %08X %08X\n",__LINE__,gWPADConnectCallback[0],gWPADConnectCallback[1],gWPADConnectCallback[2],gWPADConnectCallback[3]);
        if(HID_DEBUG) log_printf("my_VPADRead(line %d): gKPADConnectCallback   =  %08X %08X %08X %08X\n",__LINE__,gKPADConnectCallback[0],gKPADConnectCallback[1],gKPADConnectCallback[2],gKPADConnectCallback[3]);

        for(s32 i = 0;i<4;i++){
            bool doCall = false;
            if(i == 0){ doCall = ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1); }
            if(i == 1){ doCall = ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2); }
            if(i == 2){ doCall = ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3); }
            if(i == 3){ doCall = ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4); }
            if(doCall){
                if(gWPADConnectCallback[i] != NULL){
                    log_printf("my_VPADRead(line %d): Called WPAD connect callback for pro controller in slot %d!\n",__LINE__,(i+1));
                    gWPADConnectCallback[i](i,0);
                }
                if(gKPADConnectCallback[i] != NULL){
                    log_printf("my_VPADRead(line %d): Called KPAD connect callback for pro controller in slot %d!\n",__LINE__,(i+1));
                    gKPADConnectCallback[i](i,0);
                }
                if(gExtensionCallback[i] != NULL){
                    log_printf("my_VPADRead(line %d): Called extension callback for pro controller in slot %d!\n",__LINE__,(i+1));
                    gExtensionCallback[i](i,WPAD_EXT_PRO_CONTROLLER);
                }
            }
        }
    }
    if(gCallbackCooldown > 0){
        gCallbackCooldown--;
    }

    if(gHIDAttached && buffer_size > 0){
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

    if(gButtonRemappingConfigDone){
        ControllerPatcher::buttonRemapping(buffer,result);
        //ControllerPatcher::printVPADButtons(buffer); //Leads to random crashes on app transitions.
    }

    return result;
}

DECL(s32, KPADRead, s32 chan, KPADData * buffer, u32 buffer_count){
    if(buffer_count > 0){
        s32 res = ControllerPatcher::setProControllerDataFromHID((void*)&buffer[0],chan,PRO_CONTROLLER_MODE_KPADDATA); //Check if a controller is connected and fill the buffer with data.
        if(res >= 0){

            if(buffer[0].pro.btns_h & WPAD_PRO_BUTTON_HOME){ //Pro controller doesn't work in home menu so it's okay to let this enabled.
                OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }
            return 1; // We have saved one set of data. Should be enough because its no wireless communication.
        }else if(res != CONTROLLER_PATCHER_ERROR_MAPPING_DISABLED){
            //log_printf("KPADRead error: %d\n",res);
        }
    }

    return real_KPADRead(chan,buffer,buffer_count);
}

DECL(s32,KPADReadEx, s32 chan, KPADData * buffer, u32 buffer_count, s32 *error ){
    if(buffer_count > 0){
        s32 res = ControllerPatcher::setProControllerDataFromHID((void*)&buffer[0],chan,PRO_CONTROLLER_MODE_KPADDATA); //Check if a controller is connected and fill the buffer with data.
        if(res >= 0){
            //if(chan == 0) log_printf("my   %08X %08X %08X\n",buffer->pro.btns_h,buffer->pro.btns_d,buffer->pro.btns_r);
            if(buffer[0].pro.btns_h & WPAD_PRO_BUTTON_HOME){ //Pro controller doesn't work in home menu so it's okay to let this enabled.
                OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }
            if(error != NULL){
                *error = 0;
            }
            return 1; // We have saved one set of data. Should be enough because its no wireless communication.
        }else if(res != CONTROLLER_PATCHER_ERROR_MAPPING_DISABLED){
            //log_printf("KPADReadEx error: %d\n",res);
        }
    }
    return real_KPADReadEx(chan,buffer,buffer_count,error);
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
    gWPADConnectCallback[chan] = callback;
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
    gExtensionCallback[chan] = callback;
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
    gKPADConnectCallback[chan] = callback;
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
    u32 result = real_WPADGetDataFormat(chan);
    //log_printf("WPADGetDataFormat chan: %d result: %d\n",chan,result);
    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
    (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
    (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
    (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
         result = WPAD_FMT_PRO_CONTROLLER;
    }

    return result;
}

DECL(s32, WPADSetDataFormat, s32 chan, u32 fmt){
    s32 result = real_WPADSetDataFormat(chan,fmt);
    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
    (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
    (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
    (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
         if(fmt == WPAD_FMT_PRO_CONTROLLER ){
            result = 0;
         }
    }
    // log_printf("WPADSetDataFormat chan: %d fmt %d result: %d\n",chan,fmt,result);
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

DECL(void,KPADGetUnifiedWpadStatus,s32 chan, KPADUnifiedWpadData * data, void * unknown ){
    if((chan == 0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
        (chan == 1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
        (chan == 2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
        (chan == 3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))){
            ControllerPatcher::setProControllerDataFromHID((void*)data,chan,PRO_CONTROLLER_MODE_WPADReadData);
    }else{
        real_KPADGetUnifiedWpadStatus(chan,data,unknown);
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

hooks_magic_t method_hooks_hid_controller[] __attribute__((section(".data"))) = {
    MAKE_MAGIC(VPADRead,                            LIB_VPAD,       STATIC_FUNCTION),
    MAKE_MAGIC(GX2CopyColorBufferToScanBuffer,      LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(__PPCExit,                           LIB_CORE_INIT,  STATIC_FUNCTION),
    MAKE_MAGIC(WPADGetBatteryLevel,                 LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADSetConnectCallback,              LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADSetConnectCallback,                 LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADSetExtensionCallback,            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADGetUnifiedWpadStatus,            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADRead,                            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADReadEx,                          LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADRead,                            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADGetDataFormat,                   LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADSetDataFormat,                   LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADControlMotor,                    LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADProbe,                           LIB_PADSCORE,   DYNAMIC_FUNCTION),
};

u32 method_hooks_size_hid_controller __attribute__((section(".data"))) = sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile u32 method_calls_hid_controller[sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

