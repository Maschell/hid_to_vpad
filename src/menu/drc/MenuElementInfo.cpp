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
#include "MenuElementInfo.h"
#include "content/ContentTemplate.h"
#include "content/ContentHome.h"
#include "content/ContentNetworkHelp.h"
#include "content/ContentHelp.h"
#include "content/ContentSettings.h"
#include "content/ContentAbout.h"

/**
 * Constructor for the MenuElementInfo class.
 */

MenuElementInfo::MenuElementInfo(std::string _title,std::string iconPath,ContentTemplate::eContentNames _contentName)
 : MenuElement(450,84)
    ,icon_imgdata(Resources::GetImageData(iconPath.c_str()))
    ,icon_img(icon_imgdata)
    ,bg_imgdata(Resources::GetImageData("ItemBackgroundSmall.png"))
    ,bg_img(bg_imgdata)
    ,highlighted_imgdata(Resources::GetImageData("selectorSmall.png"))
    ,highlighted_img(highlighted_imgdata)
{
    contentName = _contentName;
    setTitle(_title);

    icon_img.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    icon_img.setPosition(20+18,0);
    icon_img.setParent(this);

    title.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    title.setPosition(100,0);
    title.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));
    title.setParent(&icon_img);

    bg_img.setScale(height/bg_img.getHeight());
    highlighted_img.setScale(height/highlighted_img.getHeight());

    setImage(&bg_img);
    setIconOver(&highlighted_img);
}

void MenuElementInfo::setTitle(std::string newTitle){
    title.setText(newTitle.c_str());
}

/**
 * Destructor for the MenuElementInfo class.
 */
MenuElementInfo::~MenuElementInfo()
{
    Resources::RemoveImageData(bg_imgdata);
    Resources::RemoveImageData(highlighted_imgdata);
    Resources::RemoveImageData(icon_imgdata);
}

//If you call this function, please also delete the object. This class isn't taking care of it
ContentTemplate * MenuElementInfo::createNewContent(){
    switch(contentName){
        case ContentTemplate::CONTENT_HOME: {           return new ContentHome();  break;}
        case ContentTemplate::CONTENT_ABOUT: {          return new ContentAbout(); break;}
        case ContentTemplate::CONTENT_NETWORK_HELP: {   return new ContentNetworkHelp();  break;}
        case ContentTemplate::CONTENT_SETTINGS: {       return new ContentSettings();  break;}
        case ContentTemplate::CONTENT_HELP: {           return new ContentHelp();  break;}
        default: return NULL;
    }
}

void MenuElementInfo::update(GuiController * c){
    MenuElement::update(c);
}

void MenuElementInfo::draw(CVideo *v){
    MenuElement::draw(v);
    icon_img.draw(v);
    title.draw(v);
}


