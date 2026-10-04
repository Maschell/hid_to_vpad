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
#include "utils/StringTools.h"
#include "utils/logger.h"
#include <controller_patcher/ControllerPatcher.hpp>
#include <coreinit/debug.h>
#include <wups.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config_api.h>
#include <wups/storage.h>

bool runNetworkClient = true;

void loadMapping(const std::string &persistedValue, UController_Type type);

void ConfigLoad() {
    bool rumble = false;
    WUPSStorageAPI::GetOrStoreDefault("rumble", rumble, false);
    DEBUG_FUNCTION_LINE("Set rumble to %d", rumble);
    ControllerPatcher::setRumbleActivated(rumble);

    WUPSStorageAPI::GetOrStoreDefault("networkclient", runNetworkClient, true);

    char buffer[512];

    buffer[0] = '\0';
    if (WUPSStorageAPI_GetString(nullptr, "gamepadmapping", buffer, sizeof(buffer), nullptr) == WUPS_STORAGE_ERROR_SUCCESS && buffer[0] != '\0') {
        loadMapping(std::string(buffer), UController_Type_Gamepad);
    }
    buffer[0] = '\0';
    if (WUPSStorageAPI_GetString(nullptr, "pro1", buffer, sizeof(buffer), nullptr) == WUPS_STORAGE_ERROR_SUCCESS && buffer[0] != '\0') {
        loadMapping(std::string(buffer), UController_Type_Pro1);
    }
    buffer[0] = '\0';
    if (WUPSStorageAPI_GetString(nullptr, "pro2", buffer, sizeof(buffer), nullptr) == WUPS_STORAGE_ERROR_SUCCESS && buffer[0] != '\0') {
        loadMapping(std::string(buffer), UController_Type_Pro2);
    }
    buffer[0] = '\0';
    if (WUPSStorageAPI_GetString(nullptr, "pro3", buffer, sizeof(buffer), nullptr) == WUPS_STORAGE_ERROR_SUCCESS && buffer[0] != '\0') {
        loadMapping(std::string(buffer), UController_Type_Pro3);
    }
    buffer[0] = '\0';
    if (WUPSStorageAPI_GetString(nullptr, "pro4", buffer, sizeof(buffer), nullptr) == WUPS_STORAGE_ERROR_SUCCESS && buffer[0] != '\0') {
        loadMapping(std::string(buffer), UController_Type_Pro4);
    }
}

void loadMapping(const std::string &persistedValue, UController_Type controllerType) {
    if (persistedValue.empty()) {
        return;
    }
    std::vector<std::string> result = StringTools::stringSplit(persistedValue, ",");
    if (result.size() != 4) {
        return;
    }

    ControllerMappingPADInfo mappedPadInfo;
    mappedPadInfo.vidpid.vid = atoi(result.at(0).c_str());
    mappedPadInfo.vidpid.pid = atoi(result.at(1).c_str());
    mappedPadInfo.pad        = atoi(result.at(2).c_str());
    mappedPadInfo.type       = CM_Type_Controller;

    ControllerPatcher::addControllerMapping(controllerType, mappedPadInfo);
}

void rumbleChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE("rumbleChanged %d", newValue);
    ControllerPatcher::setRumbleActivated(newValue);
    WUPSStorageAPI::Store("rumble", newValue);
}

void networkClientChanged(ConfigItemBoolean *item, bool newValue) {
    DEBUG_FUNCTION_LINE("Trigger network %d", newValue);
    runNetworkClient = newValue;
    ControllerPatcher::setNetworkControllerActivated(newValue);
    if (newValue) {
        ControllerPatcher::startNetworkServer();
    } else {
        ControllerPatcher::stopNetworkServer();
    }
    WUPSStorageAPI::Store("networkclient", newValue);
}

void PadMappingUpdated(ConfigItemPadMapping *item) {
    ControllerPatcher::resetControllerMapping(item->controllerType);
    if (item->mappedPadInfo.active && item->mappedPadInfo.type == CM_Type_Controller) {
        auto res = StringTools::strfmt("%d,%d,%d,%d", item->mappedPadInfo.vidpid.vid, item->mappedPadInfo.vidpid.pid, item->mappedPadInfo.pad, item->mappedPadInfo.type);
        WUPSStorageAPI_StoreString(nullptr, item->configId, res.c_str());
        loadMapping(res, item->controllerType);
        return;
    }
    WUPSStorageAPI_StoreString(nullptr, item->configId, "");
}

bool gConfigMenuOpen = false;

WUPSConfigAPICallbackStatus configMenuOpenedCallback(WUPSConfigCategoryHandle root) {
    gConfigMenuOpen = true;

    WUPSConfigCategoryHandle catMapping;
    WUPSConfigCategoryHandle catOther;

    if (WUPSConfigAPI_Category_Create({.name = "Mapping"}, &catMapping) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to create Mapping category");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }
    if (WUPSConfigAPI_Category_Create({.name = "Other"}, &catOther) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to create Other category");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    if (WUPSConfigItemBoolean_AddToCategoryEx(catOther, "rumble", "Rumble",
                                              false, ControllerPatcher::isRumbleActivated(),
                                              &rumbleChanged, "On", "Off") != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to add rumble item");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }
    if (WUPSConfigItemBoolean_AddToCategoryEx(catOther, "networkclient", "Network Client",
                                              true, runNetworkClient,
                                              &networkClientChanged, "On", "Off") != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to add networkclient item");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    if (!WUPSConfigItemPadMapping_AddToCategory(catMapping, "gamepadmapping", "Gamepad", UController_Type_Gamepad, &PadMappingUpdated) ||
        !WUPSConfigItemPadMapping_AddToCategory(catMapping, "pro1", "Pro Controller 1", UController_Type_Pro1, &PadMappingUpdated) ||
        !WUPSConfigItemPadMapping_AddToCategory(catMapping, "pro2", "Pro Controller 2", UController_Type_Pro2, &PadMappingUpdated) ||
        !WUPSConfigItemPadMapping_AddToCategory(catMapping, "pro3", "Pro Controller 3", UController_Type_Pro3, &PadMappingUpdated) ||
        !WUPSConfigItemPadMapping_AddToCategory(catMapping, "pro4", "Pro Controller 4", UController_Type_Pro4, &PadMappingUpdated)) {
        DEBUG_FUNCTION_LINE("Failed to add pad mapping item");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    if (WUPSConfigAPI_Category_AddCategory(root, catMapping) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to add Mapping category to root");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }
    if (WUPSConfigAPI_Category_AddCategory(root, catOther) != WUPSCONFIG_API_RESULT_SUCCESS) {
        DEBUG_FUNCTION_LINE("Failed to add Other category to root");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void configMenuClosedCallback() {
    gConfigMenuOpen = false;
    WUPSStorageAPI::SaveStorage();
}
