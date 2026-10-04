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
#include <coreinit/debug.h>
#include <padscore/wpad.h>
#include <set>
#include <stdio.h>
#include <string.h>
#include <utils/StringTools.h>
#include <utils/logger.h>
#include <vector>
#include <vpad/input.h>
#include <wups/config_api.h>

// At this point the VPADRead function is already patched. But we want to use the original function (note: this could be patched by a different plugin)
typedef int32_t (*VPADReadFunction)(VPADChan chan, VPADStatus *buffer, uint32_t buffer_size, VPADReadError *error);
extern VPADReadFunction real_VPADRead;

bool WUPSConfigItemPadMapping_callCallback(void *context) {
    auto *item = (ConfigItemPadMapping *) context;
    if (item->callback != nullptr) {
        ((ConfigItemPadMappingChangedCallback) (item->callback))(item);
        return true;
    }
    return false;
}

void restoreDefault(void *context) {
    auto *item = (ConfigItemPadMapping *) context;
    memset(&item->mappedPadInfo, 0, sizeof(item->mappedPadInfo));
}

bool updatePadInfo(ConfigItemPadMapping *item) {
    int32_t found = ControllerPatcher::getActiveMappingSlot(item->controllerType);
    if (found != -1) {
        ControllerMappingPADInfo *info = ControllerPatcher::getControllerMappingInfo(item->controllerType, found);
        if (info != nullptr) {
            memcpy(&item->mappedPadInfo, info, sizeof(item->mappedPadInfo));
            return true;
        }
    } else {
        restoreDefault(item);
        return false;
    }
    return false;
}


void checkForInput(ConfigItemPadMapping *item) {
    int32_t inputsize = gHIDMaxDevices;
    auto *hiddata     = (InputData *) malloc(sizeof(InputData) * inputsize);
    memset(hiddata, 0, sizeof(InputData) * inputsize);

    ControllerMappingPADInfo pad_result = {};
    bool gotPress                       = false;

    VPADStatus vpad_data = {};
    VPADReadError error;

    // Only accept a press that starts after we began waiting. A device that already reports a held
    // button (e.g. a wireless receiver with no controller paired) would otherwise be bound instantly.
    std::set<uint64_t> heldLastPoll;
    bool firstPoll = true;

    bool unmap = false;
    while (!gotPress) {
        real_VPADRead(VPAD_CHAN_0, &vpad_data, 1, &error);
        if (error == VPAD_READ_SUCCESS) {
            if (vpad_data.hold == VPAD_BUTTON_X) {
                unmap = true;
                break;
            }
            if (vpad_data.hold == VPAD_BUTTON_B || vpad_data.hold == VPAD_BUTTON_HOME) {
                break;
            }
        }

        int32_t result = ControllerPatcher::gettingInputAllDevices(hiddata, inputsize);
        std::set<uint64_t> heldNow;
        for (int32_t i = 0; i < result; i++) {
            for (int32_t j = 0; j < HID_MAX_PADS_COUNT; j++) {
                if (hiddata[i].button_data[j].btn_h == 0) {
                    continue;
                }
                uint64_t key = ((uint64_t) hiddata[i].device_info.slotdata.hidmask << 8) | (uint64_t) j;
                heldNow.insert(key);
                if (!gotPress && !firstPoll && heldLastPoll.count(key) == 0) {
                    pad_result.pad        = j;
                    pad_result.vidpid.vid = hiddata[i].device_info.vidpid.vid;
                    pad_result.vidpid.pid = hiddata[i].device_info.vidpid.pid;
                    pad_result.active     = 1;
                    pad_result.type       = hiddata[i].type;

                    gotPress = true;
                    DEBUG_FUNCTION_LINE("%04X %04X (PAD: %d) pressed a buttons %08X", pad_result.vidpid.vid, pad_result.vidpid.pid, pad_result.pad, hiddata[i].button_data[j].btn_h);
                }
            }
        }
        heldLastPoll = heldNow;
        firstPoll    = false;
    }
    if (gotPress) {
        ControllerPatcher::resetControllerMapping(item->controllerType);
        ControllerPatcher::addControllerMapping(item->controllerType, pad_result);
        updatePadInfo(item);
        WUPSConfigItemPadMapping_callCallback(item);
    } else if (unmap) {
        ControllerPatcher::resetControllerMapping(item->controllerType);
        updatePadInfo(item);
        WUPSConfigItemPadMapping_callCallback(item);
    }

    free(hiddata);
}

static std::string GetDeviceName(ConfigItemPadMapping *item) {
    if (!updatePadInfo(item)) {
        return "No Device";
    }

    std::string name;
    std::string isConnectedString = "attached";

    if (!ControllerPatcher::isControllerConnectedAndActive(item->controllerType)) {
        isConnectedString = "detached";
    }

    ControllerMappingPADInfo *info = &item->mappedPadInfo;

    if (info->type == CM_Type_Controller) {
        std::string titleString = ControllerPatcher::getIdentifierByVIDPID(info->vidpid.vid, info->vidpid.pid);
        name                    = StringTools::strfmt("%s (%d) %s", titleString.c_str(), info->pad, isConnectedString.c_str());
    } else if (info->type == CM_Type_RealController) {
        // currently this case can't happen.
        name = "Real (Pro) Controller";
    } else if (info->type == CM_Type_Mouse || info->type == CM_Type_Keyboard) {
        // currently this case can't happen.
        name = "Mouse / Keyboard";
    }
    return name;
}

int32_t WUPSConfigItemPadMapping_getCurrentValueDisplaySelected(void *context, char *out_buf, int32_t out_size) {
    auto *item = (ConfigItemPadMapping *) context;
    if (item->state == CONFIG_ITEM_PAD_MAPPING_PREPARE_FOR_HOLD || item->state == CONFIG_ITEM_PAD_MAPPING_WAIT_FOR_HOLD) {
        if (item->state == CONFIG_ITEM_PAD_MAPPING_PREPARE_FOR_HOLD) {
            item->state = CONFIG_ITEM_PAD_MAPPING_WAIT_FOR_HOLD;
            snprintf(out_buf, out_size, "<Waiting for input on HID> (\ue001: Abort, \ue002 Reset)");
            return 0;
        } else {
            checkForInput(item);
            item->state = CONFIG_ITEM_PAD_MAPPING_STATE_NONE;
        }
    }
    snprintf(out_buf, out_size, "> %s", GetDeviceName(item).c_str());
    return 0;
}

int32_t WUPSConfigItemPadMapping_getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
    auto *item       = (ConfigItemPadMapping *) context;
    std::string name = GetDeviceName(item);
    snprintf(out_buf, out_size, " %s", GetDeviceName(item).c_str());
    return 0;
}

void WUPSConfigItemPadMapping_onButtonPressed(void *context, WUPSConfigButtons buttons) {
    auto *item = (ConfigItemPadMapping *) context;
    if (item->state == CONFIG_ITEM_PAD_MAPPING_STATE_NONE) {
        if ((buttons & WUPS_CONFIG_BUTTON_A) == WUPS_CONFIG_BUTTON_A) {
            item->state = CONFIG_ITEM_PAD_MAPPING_PREPARE_FOR_HOLD;
        }
    }
}

bool WUPSConfigItemPadMapping_isMovementAllowed(void *context) {
    return true;
}


void WUPSConfigItemPadMapping_onSelected(void *context, bool isSelected) {
}


void WUPSConfigItemPadMapping_onDelete(void *context) {
    auto *item = (ConfigItemPadMapping *) context;
    if (item->configId) {
        free(item->configId);
    }
    free(item);
}


extern "C" bool WUPSConfigItemPadMapping_AddToCategory(WUPSConfigCategoryHandle cat, const char *configID, const char *displayName, UController_Type controllerType, ConfigItemPadMappingChangedCallback callback) {
    if (cat == 0 || displayName == nullptr) {
        return false;
    }
    auto *item = (ConfigItemPadMapping *) malloc(sizeof(ConfigItemPadMapping));
    if (item == nullptr) {
        OSReport("WUPSConfigItemPadMapping_AddToCategory: Failed to allocate memory for item data.\n");
        return false;
    }

    if (configID != nullptr) {
        item->configId = strdup(configID);
    } else {
        item->configId = nullptr;
    }

    item->controllerType = controllerType;
    item->callback       = (void *) callback;
    item->state          = CONFIG_ITEM_PAD_MAPPING_STATE_NONE;
    memset(&item->mappedPadInfo, 0, sizeof(item->mappedPadInfo));

    WUPSConfigAPIItemCallbacksV1 callbacks = {
            .getCurrentValueDisplay         = &WUPSConfigItemPadMapping_getCurrentValueDisplay,
            .getCurrentValueSelectedDisplay = &WUPSConfigItemPadMapping_getCurrentValueDisplaySelected,
            .onSelected                     = &WUPSConfigItemPadMapping_onSelected,
            .restoreDefault                 = &restoreDefault,
            .isMovementAllowed              = &WUPSConfigItemPadMapping_isMovementAllowed,
            .callCallback                   = &WUPSConfigItemPadMapping_callCallback,
            .onButtonPressed                = &WUPSConfigItemPadMapping_onButtonPressed,
            .onDelete                       = &WUPSConfigItemPadMapping_onDelete};

    WUPSConfigAPIItemOptionsV1 options = {
            .configId    = configID,
            .displayName = displayName,
            .context     = item,
            .callbacks   = callbacks};

    WUPSConfigAPICreateItemOptions itemOptions;
    itemOptions.version = WUPS_API_ITEM_OPTION_VERSION_V1;
    itemOptions.data.v1 = options;

    if (WUPSConfigAPI_Item_CreateEx(itemOptions, &item->handle) != WUPSCONFIG_API_RESULT_SUCCESS) {
        free(item);
        return false;
    }

    if (WUPSConfigAPI_Category_AddItem(cat, item->handle) != WUPSCONFIG_API_RESULT_SUCCESS) {
        return false;
    }
    return true;
}