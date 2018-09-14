/****************************************************************************
 * Copyright (C) 2018 Maschell
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
#include <wups.h>

#include <controller_patcher/ControllerPatcher.hpp>
#include <vpad/input.h>
#include <padscore/wpad.h>

DECL_FUNCTION(int32_t, VPADRead, VPADChan chan, VPADStatus *buffer, uint32_t buffer_size, VPADReadError *error) {
    int32_t result = real_VPADRead(chan, buffer, buffer_size, error);
    //A keyboard only sends data when the state changes. We force it to call the sampling callback on each frame!
    ControllerPatcher::sampleKeyboardData();

    bool do_callback = (result > 0 && (buffer[0].hold & VPAD_BUTTON_TV));
    ControllerPatcher::handleCallbackData(do_callback);

    if(ControllerPatcher::areControllersConnected() && buffer_size > 0) {
        ControllerPatcher::setRumble(UController_Type_Gamepad,!!VPADBASEGetMotorOnRemainingCount(VPAD_CHAN_0));

        if(ControllerPatcher::setControllerDataFromHID(buffer) == CONTROLLER_PATCHER_ERROR_NONE) {

            if(buffer[0].hold & VPAD_BUTTON_HOME) {
                //You can open the home menu this way, but not close it. Need a proper way to close it using the same button...
                //OSSendAppSwitchRequest(5,0,0); //Open the home menu!
            }

            if(error != NULL) {
                *error = VPAD_READ_SUCCESS;
            }
            result = 1; // We want the WiiU to ignore everything else.
        }
    }

    if(ControllerPatcher::isButtonRemappingDone()) {
        ControllerPatcher::buttonRemapping(buffer,result);
        //ControllerPatcher::printVPADButtons(buffer); //Leads to random crashes on app transitions.
    }

    return result;
}

DECL_FUNCTION(int32_t, WPADProbe, WPADChan chan, uint32_t * result ) {
    if( (chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        if(result != NULL) {
            *result = WPAD_EXT_PRO_CONTROLLER;
        }
        return 0;
    }

    return real_WPADProbe(chan,result);
}

DECL_FUNCTION(WPADConnectCallback,WPADSetConnectCallback,WPADChan chan, WPADConnectCallback callback ) {
    //log_printf("WPADSetConnectCallback chan %d %08X\n",chan,callback);

    ControllerPatcher::setWPADConnectCallback(chan,callback);

    if( (chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        if(callback != NULL) {
            callback(chan,0);
        }
    }
    return real_WPADSetConnectCallback(chan,callback);
}

DECL_FUNCTION(WPADExtensionCallback,WPADSetExtensionCallback,WPADChan chan, WPADExtensionCallback callback ) {
    //log_printf("WPADSetExtensionCallback chan %d %08X\n",chan,callback);

    ControllerPatcher::setKPADExtensionCallback(chan,callback);

    if((chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        if(callback != NULL) {
            callback(chan,WPAD_EXT_PRO_CONTROLLER);
        }
    }
    return real_WPADSetExtensionCallback(chan,callback);
}

DECL_FUNCTION(WPADConnectCallback,KPADSetConnectCallback,WPADChan chan, WPADConnectCallback callback ) {
    //log_printf("KPADSetConnectCallback chan %d %08X\n",chan,callback);

    ControllerPatcher::setKPADConnectedCallback(chan,callback);

    if( (chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        if(callback != NULL) {
            callback(chan,0);
        }
    }
    return real_KPADSetConnectCallback(chan,callback);
}

DECL_FUNCTION(uint8_t, WPADGetBatteryLevel, WPADChan chan) {
    uint8_t result = real_WPADGetBatteryLevel(chan);

    if( (chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        result = 4; // Full battery
    }
    return result;
}

//In case a game relies on this...
DECL_FUNCTION(uint32_t, WPADGetDataFormat, WPADChan chan) {
    //log_printf("WPADGetDataFormat chan: %d result: %d\n",chan,result);
    if((chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        return WPAD_FMT_PRO_CONTROLLER;
    }
    return real_WPADGetDataFormat(chan);
}

DECL_FUNCTION(int32_t, WPADSetDataFormat, WPADChan chan, WPADDataFormat fmt) {
    int32_t result = -1;
    if((chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        real_WPADSetDataFormat(chan,WPAD_FMT_PRO_CONTROLLER);
        result = 0;
    }
    result = real_WPADSetDataFormat(chan,fmt);
    return result;
}

DECL_FUNCTION(void,WPADRead,WPADChan chan, WPADStatusProController *data ) {
    if((chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) ||
            (chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) ||
            (chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) ||
            (chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4))) {
        ControllerPatcher::setProControllerDataFromHID((void*)data,chan,PRO_CONTROLLER_MODE_WPADReadData);
    } else {
        real_WPADRead(chan,data);

    }
}

DECL_FUNCTION(void,WPADControlMotor, WPADChan chan, uint32_t status ) {
    if(chan == WPAD_CHAN_0 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro1)) {
        ControllerPatcher::setRumble(UController_Type_Pro1,status);
    } else if(chan == WPAD_CHAN_1 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro2)) {
        ControllerPatcher::setRumble(UController_Type_Pro2,status);
    } else if(chan == WPAD_CHAN_2 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro3)) {
        ControllerPatcher::setRumble(UController_Type_Pro3,status);
    } else if(chan == WPAD_CHAN_3 && ControllerPatcher::isControllerConnectedAndActive(UController_Type_Pro4)) {
        ControllerPatcher::setRumble(UController_Type_Pro4,status);
    }
    real_WPADControlMotor(chan,status);
}

WUPS_MUST_REPLACE(VPADRead,                 WUPS_LOADER_LIBRARY_VPAD,       VPADRead);
WUPS_MUST_REPLACE(WPADGetBatteryLevel,      WUPS_LOADER_LIBRARY_PADSCORE,   WPADGetBatteryLevel);
WUPS_MUST_REPLACE(KPADSetConnectCallback,   WUPS_LOADER_LIBRARY_PADSCORE,   KPADSetConnectCallback);
WUPS_MUST_REPLACE(WPADSetConnectCallback,   WUPS_LOADER_LIBRARY_PADSCORE,   WPADSetConnectCallback);
WUPS_MUST_REPLACE(WPADSetExtensionCallback, WUPS_LOADER_LIBRARY_PADSCORE,   WPADSetExtensionCallback);
WUPS_MUST_REPLACE(WPADRead,                 WUPS_LOADER_LIBRARY_PADSCORE,   WPADRead);
WUPS_MUST_REPLACE(WPADGetDataFormat,        WUPS_LOADER_LIBRARY_PADSCORE,   WPADGetDataFormat);
WUPS_MUST_REPLACE(WPADSetDataFormat,        WUPS_LOADER_LIBRARY_PADSCORE,   WPADSetDataFormat);
WUPS_MUST_REPLACE(WPADControlMotor,         WUPS_LOADER_LIBRARY_PADSCORE,   WPADControlMotor);
WUPS_MUST_REPLACE(WPADProbe,                WUPS_LOADER_LIBRARY_PADSCORE,   WPADProbe);
