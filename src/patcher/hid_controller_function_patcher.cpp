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
    log_printf("__PPCExit\n");
    CursorDrawer::destroyInstance();

    ControllerPatcher::destroyConfigHelper();
    ControllerPatcher::stopNetworkServer();

    real___PPCExit();
}

DECL(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);


    if(gHIDAttached && buffer_size > 0){
        ControllerPatcher::setRumble(UController_Type_Gamepad,!!VPADBASEGetMotorOnRemainingCount(0));

        if(ControllerPatcher::setControllerDataFromHID(buffer) == CONTROLLER_PATCHER_ERROR_NONE){

            if(buffer[0].btns_h & VPAD_BUTTON_HOME){
                //You can open the home menu this way, but not close it. Need a proper way to close it using the same button...
                //OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }

            *error = 0;
            result = 1; // We want the WiiU to ignore everything else.
        }else{

        }
    }

    if(gButtonRemappingConfigDone){
        ControllerPatcher::buttonRemapping(buffer,result);
        //ControllerPatcher::printVPADButtons(buffer); //Leads to random crashes on app transitions.
    }

    return result;
}

DECL(s32,KPADReadEx, s32 chan, KPADData * buffer, u32 buffer_count, s32 *error ){
    //log_printf("KPADReadEx\n");
    if(buffer_count > 0){
        int res = ControllerPatcher::setProControllerDataFromHID((void*)&buffer[0],chan,PRO_CONTROLLER_MODE_KPADDATA); //Check if a controller is connected and fill the buffer with data.
        if(res >= 0){

            if(buffer[0].pro.btns_h & WPAD_PRO_BUTTON_HOME){ //Pro controller doesn't work in home menu so it's okay to let this enabled.
                OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }
            *error = 0; //No error
            return 1; // We have saved one set of data. Should be enough because its no wireless communication.
        }else if(res != CONTROLLER_PATCHER_ERROR_MAPPING_DISABLED){
            //log_printf("KPADReadEx error: %d\n",res);
        }
    }
    s32 result = real_KPADReadEx(chan,buffer,buffer_count,error);
    return result;
}

DECL(s32, WPADProbe, s32 chan, u32 * result ){
    if((ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
        *result = WPAD_EXT_PRO_CONTROLLER;
        return 0;
    }

    return real_WPADProbe(chan,result);
}

DECL(wpad_extension_callback_t,WPADSetExtensionCallback,s32 chan, wpad_extension_callback_t callback ){
    if( (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
            if(callback != NULL){
                callback(chan,WPAD_EXT_PRO_CONTROLLER);
            }
    }
    return real_WPADSetExtensionCallback(chan,callback);
}

DECL(wpad_connect_callback_t,KPADSetConnectCallback,s32 chan, wpad_connect_callback_t callback ){
    if( (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
            if(callback != NULL){
                callback(chan,0);
            }
    }
    return real_KPADSetConnectCallback(chan,callback);
}

DECL(u8, WPADGetBatteryLevel, s32 chan){
    u8 result = real_WPADGetBatteryLevel(chan);
    if((ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
        result = 4; // Full battery
    }
    return result;
}

//In case a game relies on this...
DECL(u32, WPADGetDataFormat, s32 chan){
    u32 result = real_WPADGetDataFormat(chan);

    if((ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
    (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
         result = WPAD_FMT_PRO_CONTROLLER;
    }

    return result;
}

DECL(void,WPADRead,s32 chan, WPADReadData *data ){
    if((ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
            ControllerPatcher::setProControllerDataFromHID((void*)data,chan,PRO_CONTROLLER_MODE_WPADReadData);

    }else{
        real_WPADRead(chan,data);
    }
}

DECL(void,KPADGetUnifiedWpadStatus,s32 chan, KPADUnifiedWpadData * data, void * unknown ){
    if((ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1) && chan == 0) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2) && chan == 1) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3) && chan == 2) ||
        (ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4) && chan == 3)){
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
    MAKE_MAGIC(WPADSetExtensionCallback,            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADGetUnifiedWpadStatus,            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(KPADReadEx,                          LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADRead,                            LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADGetDataFormat,                   LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADControlMotor,                    LIB_PADSCORE,   DYNAMIC_FUNCTION),
    MAKE_MAGIC(WPADProbe,                           LIB_PADSCORE,   DYNAMIC_FUNCTION),
};

u32 method_hooks_size_hid_controller __attribute__((section(".data"))) = sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_hid_controller[sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

