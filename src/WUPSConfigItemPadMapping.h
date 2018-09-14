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

#ifndef _WUPS_CONFIG_ITEM_PAD_MAPPING_H_
#define _WUPS_CONFIG_ITEM_PAD_MAPPING_H_

#include <string>
#include <vector>
#include <wups/config/WUPSConfigItem.h>
#include <controller_patcher/ControllerPatcher.hpp>

class WUPSConfigItemPadMapping;

typedef void (*PadMappingChangedCallback)(WUPSConfigItemPadMapping *, bool);

class WUPSConfigItemPadMapping : public WUPSConfigItem {
public:
    WUPSConfigItemPadMapping(std::string configID, std::string displayName, UController_Type controller);

    void checkForInput();

    virtual ~WUPSConfigItemPadMapping();

    void resetMappingAndDetachController();

    bool updatePadInfo();

    virtual std::string getCurrentValueDisplay();

    virtual std::string getCurrentValueSelectedDisplay();

    virtual void onSelected(bool isSelected);

    virtual void onButtonPressed(WUPSConfigButtons buttons);

    virtual bool isMovementAllowed();

    virtual std::string persistValue();

    virtual void loadValue(std::string persistedValue);

    virtual void restoreDefault();

    virtual bool callCallback();

private:
    UController_Type controllerType = UController_Type_Gamepad;
    ControllerMappingPADInfo mappedPadInfo;
};

#endif
