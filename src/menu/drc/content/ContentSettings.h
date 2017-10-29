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
#ifndef _CONTENT_SETTINGS_H
#define _CONTENT_SETTINGS_H

#include <map>

#include <gui/Gui.h>
#include <gui/DefaultGuiCheckbox.h>
#include <gui/DefaultGuiSwitch.h>
#include "ContentTemplate.h"
#include <language/gettext.h>

class ContentSettings : public ContentTemplate{
public:
    ContentSettings();
    virtual ~ContentSettings();

private:
    GuiTrigger touchTrigger;
    GuiTrigger wpadTouchTrigger;
    GuiSelectBox languageSelectBox;

    std::map<std::string,std::string> languagesNames;

    GuiFrame settingsFrame;

    GuiText headLine;

    DefaultGuiSwitch rumbleSwitch;
    DefaultGuiSwitch musicSwitch;
    DefaultGuiCheckBox networkControllerSwitch;

    GuiSound *buttonClickSound;

    std::map<std::string,GuiElement*> settings;
    std::vector<std::string> settingsOrder;
    std::map<GuiElement*,GuiFrame *> settingsFrames;

    std::vector<GuiElement*> toDelete;

    bool bChanged = false;

    void OnSelectBoxValueChanged(GuiSelectBox * selectBox, std::string value);

    void OnMusicValueChanged(GuiToggle * toggle,bool value);
    void OnRumbleValueChanged(GuiToggle * toggle,bool value);
    void OnNetworkControllerValueChanged(GuiToggle * toggle,bool value);

};

#endif //_CONTENT_SETTINGS_H
