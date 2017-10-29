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
#include <fs/DirList.h>
#include <utils/StringTools.h>

#include "ContentSettings.h"
#include "settings/CSettings.h"
#include "common/common.h"
#include "Application.h"

ContentSettings::ContentSettings():ContentTemplate()
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
    , languageSelectBox("",NULL)
    , rumbleSwitch(CSettings::instance()->getValueAsBool(CSettings::RumbleActivated),2.0f)
    , musicSwitch(CSettings::instance()->getValueAsBool(CSettings::MusicActivated),2.0f)
    , networkControllerSwitch(CSettings::instance()->getValueAsBool(CSettings::NetworkControllerActivated),4.0f)
    , buttonClickSound(Resources::GetSound("settings_click_2.mp3"))
    {
    headLine.setText(gettext("Settings"));
    headLine.setFontSize(60);
    headLine.setAlignment(ALIGN_TOP_CENTER);
    headLine.setPosition(0,-110);
    headLine.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    append(&headLine);

    settingsFrame.setAlignment(ALIGN_TOP_CENTER);
    settingsFrame.setPosition(0,-80);
    settingsFrame.setSize(getWidth(),getHeight());
    append(&settingsFrame);

    settings[gettext("Language")] = &languageSelectBox;
    settings[gettext("Rumble")] = &rumbleSwitch;
    settings[gettext("Music")] = &musicSwitch;
    settings[gettext("Network Controller")] = &networkControllerSwitch;

    settingsOrder.push_back(gettext("Language"));
    settingsOrder.push_back(gettext("Rumble"));
    settingsOrder.push_back(gettext("Music"));
    settingsOrder.push_back(gettext("Network Controller"));

    rumbleSwitch.setTrigger(&touchTrigger);
    rumbleSwitch.setTrigger(&wpadTouchTrigger);
    rumbleSwitch.setSoundClick(buttonClickSound);
    rumbleSwitch.valueChanged.connect(this, &ContentSettings::OnRumbleValueChanged);

    musicSwitch.setTrigger(&touchTrigger);
    musicSwitch.setTrigger(&wpadTouchTrigger);
    musicSwitch.setSoundClick(buttonClickSound);
    musicSwitch.valueChanged.connect(this, &ContentSettings::OnMusicValueChanged);

    networkControllerSwitch.setTrigger(&touchTrigger);
    networkControllerSwitch.setTrigger(&wpadTouchTrigger);
    networkControllerSwitch.setSoundClick(buttonClickSound);
    networkControllerSwitch.valueChanged.connect(this, &ContentSettings::OnNetworkControllerValueChanged);

    DirList dirList(DEFAULT_LANG_PATH, LANGUAGE_FILE_EXT, DirList::Files);
    dirList.SortList();

    //Fill in the values for the select box
    languagesNames[gettext("<Default>")] = "none";
    int selectedID = 0;
    std::string setting = CSettings::instance()->getValueAsString(CSettings::AppLanguage);
    for(int i = 0; i < dirList.GetFilecount(); i++){
        const char *filename = dirList.GetFilename(i);
        std::string name(filename);

        name = name.substr(0,name.length()-strlen(LANGUAGE_FILE_EXT));
        std::string displayname = name;
        displayname[0] = toupper(displayname[0]);
        languagesNames[displayname] = name;
        if(name.compare(setting) == 0) selectedID = i+1;
    }

    languageSelectBox.Init(languagesNames,selectedID);
    languageSelectBox.valueChanged.connect(this, &ContentSettings::OnSelectBoxValueChanged);

    f32 frameoffset = 0;
    f32 frameheight = 80.0f;
    for (std::vector<std::string>::iterator it = settingsOrder.begin() ; it != settingsOrder.end(); ++it){
        std::string name = *it;
        GuiElement * element =settings[*it];
        GuiFrame * frame = new GuiFrame();

        GuiFrame * left = new GuiFrame();
        GuiFrame * right = new GuiFrame();

        frame->append(left);
        frame->append(right);
        frame->setAlignment(ALIGN_TOP_CENTER);
        frame->setSize(getWidth()*0.80f,frameheight);

        GuiText * text = new GuiText(StringTools::strfmt("%s:",name.c_str()).c_str());
        text->setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
        text->setFontSize(40);
        text->setAlignment(ALIGN_LEFT);
        left->append(text);
        left->setAlignment(ALIGN_LEFT);
        right->setAlignment(ALIGN_RIGHT);
        element->setAlignment(ALIGN_RIGHT);
        element->setPosition(0,10);

        right->append(element);

        frameoffset -= frameheight;
        frame->setPosition(0,frameoffset);
        settingsFrame.append(frame);
        settingsFrames[element] = frame;

        toDelete.push_back(frame);
        toDelete.push_back(left);
        toDelete.push_back(right);
        toDelete.push_back(text);
    }
    settingsFrame.bringToFront(settingsFrames.at(&languageSelectBox));
}

ContentSettings::~ContentSettings(){
    remove(&headLine);
    Resources::RemoveSound(buttonClickSound);

    if(bChanged){
        CSettings::instance()->Save();
    }

    for (std::vector<GuiElement*>::iterator it = toDelete.begin() ; it != toDelete.end(); ++it){
        GuiElement * element = *it;
        delete element;
    }
}


void ContentSettings::OnSelectBoxValueChanged(GuiSelectBox * selectBox, std::string value){
    if(selectBox == &languageSelectBox){
        if(CSettings::getValueAsString(CSettings::AppLanguage).compare(value) != 0){
            CSettings::setValueAsString(CSettings::AppLanguage,value);
            bChanged = true;
            Application::instance()->reloadUI();
        }
    }
}

void ContentSettings::OnMusicValueChanged(GuiToggle * toggle,bool value){
    CSettings::setValueAsBool(CSettings::MusicActivated,value);
    bChanged = true;
}


void ContentSettings::OnRumbleValueChanged(GuiToggle * toggle,bool value){
    CSettings::setValueAsBool(CSettings::RumbleActivated,value);
    bChanged = true;
}

void ContentSettings::OnNetworkControllerValueChanged(GuiToggle * toggle,bool value){
    CSettings::setValueAsBool(CSettings::NetworkControllerActivated,value);
    ControllerPatcher::setNetworkControllerActivated(value);
    if(!value){
        ControllerPatcher::stopNetworkServer();
    }else{
        ControllerPatcher::startNetworkServer();
    }
    bChanged = true;
}
