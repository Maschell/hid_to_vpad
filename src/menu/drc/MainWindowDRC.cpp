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
#include "MainWindowDRC.h"
#include "../../version.h"
#include <gui/GuiControllerInputDisplay.h>

extern u32 hostIpAddress;

MainWindowDRC::MainWindowDRC(s32 w, s32 h)
    : GuiConfigurationScreen(w, h)
    , width(w)
    , height(h)
    , windowSplitter_imgdata(Resources::GetImageData("windowSplitter.png"))
    , windowSplitter_img(windowSplitter_imgdata)
    , content(1280-450-2,h)
    , menuList(450,h,&content)
    , versionText(APP_VERION,20,glm::vec4(0.3f,0.3f,0.3f,1.0f))
    , ipAddress("",20,glm::vec4(0.3f,0.3f,0.3f,1.0f))
{
    menuList.setAlignment(ALIGN_LEFT);

    windowSplitter_img.setAlignment(ALIGN_LEFT);
    windowSplitter_img.setPosition(menuList.getWidth(),0);

    content.setAlignment(ALIGN_LEFT);
    content.setPosition(menuList.getWidth()+windowSplitter_img.getWidth(),0);

    GuiControllerInputDisplay::preLoadImages();

    versionText.setText(StringTools::fmt("%s - %s",APP_VERION,__DATE__));
    versionText.setAlignment(ALIGN_TOP_LEFT);
    versionText.setPosition(windowSplitter_img.getOffsetX()+5,-25);
    ipAddress.setText(StringTools::wfmt("%s%u.%u.%u.%u",gettext("TCP Server running on: "),(hostIpAddress >> 24) & 0xFF, (hostIpAddress >> 16) & 0xFF, (hostIpAddress >> 8) & 0xFF, (hostIpAddress >> 0) & 0xFF));
    ipAddress.setAlignment(ALIGN_TOP_RIGHT);
    ipAddress.setPosition(-5,-25);
    append(&windowSplitter_img);
    append(&menuList);
    append(&content);
    append(&versionText);
    append(&ipAddress);
}

void MainWindowDRC::clickListEntryByController(UController_Type controller_type){
    MenuElement * menuElement = menuList.getButtonElementByController(controller_type);
    if(menuElement != NULL){
        menuList.OnButtonClicked(menuElement,NULL,NULL);
    }
}

void MainWindowDRC::setState(s32 i, s32 c){
    GuiFrame::setState(i,c);
}

void MainWindowDRC::process(){
    GuiConfigurationScreen::process();
}

MainWindowDRC::~MainWindowDRC(){
     remove(&menuList);
     remove(&content);
}

