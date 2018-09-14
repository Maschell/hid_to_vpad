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

#include "WUPSConfigItemPadMapping.h"
#include <controller_patcher/ControllerPatcher.hpp>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <utils/logger.h>
#include <vpad/input.h>
#include <padscore/wpad.h>
#include <utils/StringTools.h>


// At this point the VPADRead function is already patched. But we want to use the original function (note: this could be patched by a different plugin)
typedef int32_t (*VPADReadFunction) (VPADChan chan, VPADStatus *buffer, uint32_t buffer_size, VPADReadError *error);
extern VPADReadFunction real_VPADRead;

WUPSConfigItemPadMapping::WUPSConfigItemPadMapping(std::string configID, std::string displayName, UController_Type controller) : WUPSConfigItem(configID,displayName)  {
    this->controllerType = controller;
}

WUPSConfigItemPadMapping::~WUPSConfigItemPadMapping() {

}

bool WUPSConfigItemPadMapping::updatePadInfo() {
    int32_t found = ControllerPatcher::getActiveMappingSlot(controllerType);
    if(found != -1) {
        ControllerMappingPADInfo * info = ControllerPatcher::getControllerMappingInfo(controllerType,found);
        if(info != NULL) {
            memcpy(&mappedPadInfo,info,sizeof(mappedPadInfo));
            return true;
        }
    } else {
        this->restoreDefault();
        return false;
    }
    return false;
}

std::string WUPSConfigItemPadMapping::getCurrentValueDisplay() {
    if(!updatePadInfo()) {
        return "No Device";
    }
    std::string name = "";
    std::string isConnectedString = "attached";

    if(!ControllerPatcher::isControllerConnectedAndActive(controllerType)) {
        isConnectedString = "detached";
    }

    ControllerMappingPADInfo * info = &mappedPadInfo;

    if(info->type == CM_Type_Controller) {
        std::string titleString = ControllerPatcher::getIdentifierByVIDPID(info->vidpid.vid,info->vidpid.pid);
        std::vector<std::string> result = StringTools::stringSplit(titleString, "\n");
        if(result.size() == 1) {
            name = titleString;
        } else if(result.size() == 2) {
            name = StringTools::strfmt("0x%04X / 0x%04X(%d) %s",info->vidpid.vid, info->vidpid.pid, info->pad, isConnectedString.c_str());
        }
    } else if(info->type == CM_Type_RealController) {
        // currently this case can't happen.
        name = "Real (Pro) Controller";
    } else if(info->type == CM_Type_Mouse || info->type == CM_Type_Keyboard) {
        // currently this case can't happen.
        name = "Mouse / Keyboard";
    }

    return name.c_str();
}

std::string WUPSConfigItemPadMapping::getCurrentValueSelectedDisplay() {
    return getCurrentValueDisplay();
}

void WUPSConfigItemPadMapping::onSelected(bool isSelected) {

}

void WUPSConfigItemPadMapping::checkForInput() {
    int32_t inputsize = gHIDMaxDevices;
    InputData * hiddata = (InputData * ) malloc(sizeof(InputData)*inputsize);
    memset(hiddata,0,sizeof(InputData)*inputsize);

    ControllerMappingPADInfo pad_result;
    memset(&pad_result,0,sizeof(ControllerMappingPADInfo));
    bool gotPress = false;

    VPADStatus vpad_data;
    VPADReadError error;

    while(!gotPress) {
        real_VPADRead(VPAD_CHAN_0, &vpad_data, 1, &error);
        if(error != VPAD_READ_SUCCESS) {
            if(vpad_data.hold == VPAD_BUTTON_B || vpad_data.hold == VPAD_BUTTON_HOME) {
                break;
            }
        }

        int32_t result = ControllerPatcher::gettingInputAllDevices(hiddata,inputsize);
        if(result > 0) {
            //log_printf("got %d results\n",result);
            for(int32_t i =0; i<result; i++) {
                for(int32_t j = 0; j<HID_MAX_PADS_COUNT; j++) {
                    //log_printf("check pad %d. %08X\n",j,hiddata[i].button_data[j].btn_h);
                    if(hiddata[i].button_data[j].btn_h != 0) {
                        //log_printf("It pressed a buttons!\n",result);
                        pad_result.pad = j;
                        pad_result.vidpid.vid = hiddata[i].device_info.vidpid.vid;
                        pad_result.vidpid.pid = hiddata[i].device_info.vidpid.pid;
                        pad_result.active = 1;
                        pad_result.type = hiddata[i].type;

                        gotPress = true;
                        DEBUG_FUNCTION_LINE("%04X %04X (PAD: %d) pressed a buttons %08X\n",hiddata[i].device_info.vidpid.vid,hiddata[i].device_info.vidpid.pid,j,hiddata[i].button_data[j].btn_h);
                        break;
                    }
                }
                if(gotPress) {
                    break;
                }
            }
        }
    }
    if(gotPress) {
        // TODO: Save the choice and use a callback??!??!
        ControllerPatcher::addControllerMapping(this->controllerType,pad_result);
        updatePadInfo();
    }

    free(hiddata);
}

void WUPSConfigItemPadMapping::onButtonPressed(WUPSConfigButtons buttons) {
    if(buttons & WUPS_CONFIG_BUTTON_A) {
        // Lets remove the old mapping.
        ControllerPatcher::resetControllerMapping(this->controllerType);

        // And draw the text on the screen the config menu is drawn to.
        wups_overlay_options_type_t screen = this->lastVisibleOnScreen();

        WUPS_Overlay_OSScreenClear(screen);

        // TODO: Add more information about the target (e.g. is the mapping for the gamepad or pro controller 1/2/3/4)
        WUPS_Overlay_PrintTextOnScreen(screen, 0,0,"Press a button on the HID controller to map it.");
        WUPS_Overlay_PrintTextOnScreen(screen, 0,10,"Press HOME or B on the gamepad to abort.");

        WUPS_Overlay_FlipBuffers(screen);

        checkForInput();
    }
}

bool WUPSConfigItemPadMapping::isMovementAllowed() {
    return true;
}

std::string WUPSConfigItemPadMapping::persistValue() {
    updatePadInfo();
    if(mappedPadInfo.type == CM_Type_Controller){
        return StringTools::strfmt("%d,%d,%d,%d",mappedPadInfo.vidpid.vid,mappedPadInfo.vidpid.pid,mappedPadInfo.pad,mappedPadInfo.type);
    }
    return "0";
}

void WUPSConfigItemPadMapping::loadValue(std::string persistedValue) {
    if(persistedValue.compare("0") == 0) {
        // No device mapped.
        return;
    }
    std::vector<std::string> result = StringTools::stringSplit(persistedValue, ",");
    if(result.size() != 4) {
        return;
    }

    updatePadInfo();

    this->mappedPadInfo.vidpid.vid = atoi(result.at(0).c_str());
    this->mappedPadInfo.vidpid.pid = atoi(result.at(1).c_str());
    this->mappedPadInfo.pad = atoi(result.at(2).c_str());
    //Currently only persisting mapped controllers is supported.
    this->mappedPadInfo.type = CM_Type_Controller; //atoi(result.at(3).c_str());

    ControllerPatcher::resetControllerMapping(this->controllerType);
    // TODO: Save the choice and use a callback??!??!
    ControllerPatcher::addControllerMapping(this->controllerType,this->mappedPadInfo);

    return;
}

void WUPSConfigItemPadMapping::restoreDefault() {
    memset(&mappedPadInfo,0,sizeof(mappedPadInfo));
}

bool WUPSConfigItemPadMapping::callCallback() {
    // Currently we don't use a callback.
    return false;
}
