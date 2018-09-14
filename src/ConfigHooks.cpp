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
#include <wups/config.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <controller_patcher/ControllerPatcher.hpp>
#include <utils/logger.h>
#include "WUPSConfigItemPadMapping.h"

void rumbleChanged(WUPSConfigItemBoolean * item, bool newValue) {
    DEBUG_FUNCTION_LINE("rumbleChanged %d \n",newValue);
    ControllerPatcher::setRumbleActivated(newValue);
}

void networkClient(WUPSConfigItemBoolean * item, bool newValue) {
    DEBUG_FUNCTION_LINE("Trigger network %d\n",newValue);
    ControllerPatcher::setNetworkControllerActivated(newValue);
    if(newValue) {
        ControllerPatcher::startNetworkServer();
    } else {
        ControllerPatcher::stopNetworkServer();
    }
}

WUPS_GET_CONFIG() {
    WUPSConfig* config = new WUPSConfig("HID to VPAD");
    WUPSConfigCategory* catOther = config->addCategory("Other");
    WUPSConfigCategory* catMapping = config->addCategory("Controller Mapping");

    //                    item Type             config id           displayed name              default value  onChangeCallback.
    catOther->addItem(new WUPSConfigItemBoolean("rumble",           "Enable rumble",            ControllerPatcher::isRumbleActivated(), rumbleChanged));
    catOther->addItem(new WUPSConfigItemBoolean("networkclient",    "Enable network client",    true,          networkClient));

    catMapping->addItem(new WUPSConfigItemPadMapping("gamepadmapping",   "Gamepad",    UController_Type_Gamepad));
    catMapping->addItem(new WUPSConfigItemPadMapping("propad1mapping",   "Pro Controller 1",    UController_Type_Pro1));
    catMapping->addItem(new WUPSConfigItemPadMapping("propad2mapping",   "Pro Controller 2",    UController_Type_Pro2));
    catMapping->addItem(new WUPSConfigItemPadMapping("propad3mapping",   "Pro Controller 3",    UController_Type_Pro3));
    catMapping->addItem(new WUPSConfigItemPadMapping("propad4mapping",   "Pro Controller 4",    UController_Type_Pro4));

    return config;
}
