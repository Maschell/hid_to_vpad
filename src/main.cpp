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

#include <string.h>
#include <controller_patcher/ControllerPatcher.hpp>
#include <utils/logger.h>

#include <nsysnet/socket.h>
#include "WUPSConfigItemPadMapping.h"

WUPS_PLUGIN_NAME("HID to VPAD lite");
WUPS_PLUGIN_DESCRIPTION("Enables HID devices as controllers on your Wii U");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("GPL");

// We want access to the SDCard!
WUPS_FS_ACCESS()

#define SD_PATH                     "sd:"
#define WIIU_PATH                   "/wiiu"
#define DEFAULT_HID_TO_VPAD_PATH    SD_PATH WIIU_PATH "/apps/hidtovpad"

ON_APPLICATION_START(args) {
    socket_lib_init();
    log_init();

    DEBUG_FUNCTION_LINE("Initializing the controller data\n");
    ControllerPatcher::Init(CONTROLLER_PATCHER_PATH);
    ControllerPatcher::enableControllerMapping();
    DEBUG_FUNCTION_LINE("Starting HID to VPAD network server\n");
    ControllerPatcher::startNetworkServer();
    ControllerPatcher::disableWiiUEnergySetting();
}

DEINITIALIZE_PLUGIN() {
    //CursorDrawer::destroyInstance();
    ControllerPatcher::DeInit();
    ControllerPatcher::stopNetworkServer();
}

ON_APPLICATION_ENDING() {
    //CursorDrawer::destroyInstance();
    DEBUG_FUNCTION_LINE("ON_APPLICATION_ENDING\n");
    ControllerPatcher::destroyConfigHelper();
    DEBUG_FUNCTION_LINE("Calling stopNetworkServer\n");
    ControllerPatcher::stopNetworkServer();
    DEBUG_FUNCTION_LINE("Calling resetCallbackData\n");
    ControllerPatcher::resetCallbackData();
    ControllerPatcher::restoreWiiUEnergySetting();

    DEBUG_FUNCTION_LINE("Closing\n");
}
