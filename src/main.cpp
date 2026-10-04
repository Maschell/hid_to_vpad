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
#include <wups/config_api.h>

#include <controller_patcher/ControllerPatcher.hpp>
#include <cstring>
#include <utils/logger.h>

WUPSConfigAPICallbackStatus configMenuOpenedCallback(WUPSConfigCategoryHandle root);
void configMenuClosedCallback();

WUPS_PLUGIN_NAME("HID to VPAD");
WUPS_PLUGIN_DESCRIPTION("Enables HID devices as controllers on your Wii U");
WUPS_PLUGIN_VERSION("0.21-alpha");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

WUPS_USE_WUT_DEVOPTAB();
WUPS_USE_STORAGE("hid_to_vpad");

extern bool runNetworkClient;

#define SD_PATH                          "fs:/vol/external01"
#define WIIU_PATH                        "/wiiu"
#define DEFAULT_CONTROLLER_PATCHER_PATCH SD_PATH WIIU_PATH "/controller"

void ConfigLoad();
ON_APPLICATION_START() {
    WHBLogUdpInit();
    WHBLogCafeInit();

    DEBUG_FUNCTION_LINE("Initializing the controller data");
    ControllerPatcher::Init(DEFAULT_CONTROLLER_PATCHER_PATCH);
    ControllerPatcher::enableControllerMapping();

    ConfigLoad();

    if (runNetworkClient) {
        DEBUG_FUNCTION_LINE("Starting HID to VPAD network server");
        ControllerPatcher::startNetworkServer();
    }
    ControllerPatcher::disableWiiUEnergySetting();
}

INITIALIZE_PLUGIN() {
    WHBLogUdpInit();
    WUPSConfigAPI_Init({.name = "HID to VPAD"}, configMenuOpenedCallback, configMenuClosedCallback);
}

DEINITIALIZE_PLUGIN() {
    ControllerPatcher::DeInit();
    ControllerPatcher::stopNetworkServer();
}

ON_APPLICATION_REQUESTS_EXIT() {
    //CursorDrawer::destroyInstance();
    DEBUG_FUNCTION_LINE("ON_APPLICATION_ENDING");
    ControllerPatcher::destroyConfigHelper();
    DEBUG_FUNCTION_LINE("Calling stopNetworkServer");
    ControllerPatcher::stopNetworkServer();
    DEBUG_FUNCTION_LINE("Calling resetCallbackData");
    ControllerPatcher::resetCallbackData();
    ControllerPatcher::restoreWiiUEnergySetting();

    DEBUG_FUNCTION_LINE("Closing");
}
