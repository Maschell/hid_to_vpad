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
#include "ContentHome.h"

ContentHome::ContentHome():ContentTemplate()
    , logoImageData(Resources::GetImageData("logo.png"))
    , logoImage(logoImageData)
    , welcomeHeadLineLabel("Welcome to HID to VPAD!")
    , welcomeTextLabel("HID to VPAD allows you to use your USB controller on your WiiU. Currently you can emulate the Gamepad or a Pro Controller.")
    , welcomeTextLabel2("Before you use your controller, you may need to provide a valid config. More information and config files can be found in the help section or on gbatemp.net (gbatemp.net/threads/424127/).")
    , welcomeTextLabel3("To map a device, select a controller from the list on the left hand side.")
    , twitterLogoImageData(Resources::GetImageData("TwitterIcon.png"))
    , twitterLogoImage(twitterLogoImageData)
    , githubLogoImageData(Resources::GetImageData("GithubIcon.png"))
    , githubLogoImage(githubLogoImageData)
    , twitterLabel("MaschellDev")
    , URLLabel("github.com/Maschell/hid_to_vpad")
    , URL2Label("github.com/Maschell/controller_patcher")
{

    logoImage.setAlignment(ALIGN_TOP_CENTER);
    logoImage.setPosition(0,-20);

    glm::vec4 textColor = glm::vec4(0.3f,0.3f,0.3f,1.0f);

    welcomeHeadLineLabel.setColor(textColor);
    welcomeHeadLineLabel.setPosition(0,130);
    welcomeHeadLineLabel.setFontSize(40);

    welcomeTextLabel.setColor(textColor);
    welcomeTextLabel.setMaxWidth(800,GuiText::WRAP);
    welcomeTextLabel.setFontSize(25);
    welcomeTextLabel.setPosition(0,50);

    welcomeTextLabel2.setColor(textColor);
    welcomeTextLabel2.setMaxWidth(800,GuiText::WRAP);
    welcomeTextLabel2.setFontSize(25);
    welcomeTextLabel2.setPosition(0,-45);

    welcomeTextLabel3.setColor(textColor);
    welcomeTextLabel3.setMaxWidth(800,GuiText::WRAP);
    welcomeTextLabel3.setFontSize(32);
    welcomeTextLabel3.setPosition(0,-160);

    githubLogoImage.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    githubLogoImage.setPosition(240,77);

    twitterLogoImage.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    twitterLogoImage.setPosition(10,80);
    twitterLabel.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    twitterLabel.setColor(textColor);
    twitterLabel.setPosition(52,113);


    URLLabel.setColor(textColor);
    URLLabel.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    URLLabel.setPosition(280,133);

    URL2Label.setColor(textColor);
    URL2Label.setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
    URL2Label.setPosition(280,95);

    append(&logoImage);
    append(&welcomeTextLabel);
    append(&welcomeTextLabel2);
    append(&welcomeTextLabel3);
    append(&welcomeHeadLineLabel);
    append(&twitterLogoImage);
    append(&githubLogoImage);
    append(&twitterLabel);
    append(&URLLabel);
    append(&URL2Label);
}

ContentHome::~ContentHome(){
    //Resources::RemoveImageData(logoImageData);  <-- will reduce lag. Will be deleted at the end anyway
    remove(&bgImageColor);
    remove(&logoImage);
    remove(&welcomeHeadLineLabel);
    remove(&welcomeTextLabel);
    remove(&welcomeTextLabel2);
    remove(&welcomeTextLabel3);
    remove(&twitterLogoImage);
    remove(&githubLogoImage);
    remove(&twitterLabel);
    remove(&URLLabel);
    remove(&URL2Label);
}
