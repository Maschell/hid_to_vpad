/****************************************************************************
 * Copyright (C) 2015 Dimok
 * Modified by Maschell, 2016,2017 for HID to VPAD
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
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <utils/StringTools.h>
#include <utils/logger.h>

#include "MainWindowTV.h"
#include "Application.h"

TVControllerBanner *MainWindowTV::bannerInstance = NULL;

MainWindowTV::MainWindowTV(s32 w, s32 h)
    : GuiConfigurationScreen(w, h)
    , width(w)
    , height(h)
    , backgroundImageData(Resources::GetImageData("MainWindowTVBackground.jpg"))
    , backgroundImage(backgroundImageData)
    , logoImageData(Resources::GetImageData("logo.png"))
    , logoImage(logoImageData)
    , twitterLogoImageData(Resources::GetImageData("TwitterIcon.png"))
    , twitterLogoImage(twitterLogoImageData)
    , githubLogoImageData(Resources::GetImageData("GithubIcon.png"))
    , githubLogoImage(githubLogoImageData)
    , twitterLabel("twitter.com/MaschellDev")
    , githubLabel("github.com/Maschell")
    , controllerConfigurationTV(w, h)
{
    bannerInstance = &controllerConfigurationTV;

    logoImage.setAlignment(ALIGN_TOP_CENTER);
    logoImage.setPosition(0,-15);

    twitterLogoImage.setAlignment(ALIGN_TOP_LEFT);
    twitterLogoImage.setPosition(10,-10);

    twitterLabel.setAlignment(ALIGN_TOP_LEFT);
    twitterLabel.setColor(glm::vec4(0.2f,0.2f,0.2f,0.7f));
    twitterLabel.setPosition(55,-40);

    githubLogoImage.setAlignment(ALIGN_TOP_LEFT);

    githubLogoImage.setPosition(10,-45);

    githubLabel.setAlignment(ALIGN_TOP_LEFT);
    githubLabel.setColor(glm::vec4(0.2f,0.2f,0.2f,0.7f));
    githubLabel.setPosition(55,-80);

    append(&backgroundImage);
    append(&logoImage);

    append(&twitterLogoImage);
    append(&githubLogoImage);
    append(&twitterLabel);
    append(&githubLabel);
    append(&controllerConfigurationTV);
}

MainWindowTV::~MainWindowTV()
{
    remove(&controllerConfigurationTV);
    remove(&twitterLogoImage);
    remove(&logoImage);
    remove(&githubLogoImage);
    remove(&twitterLabel);
    remove(&githubLabel);
    remove(&backgroundImage);

    Resources::RemoveImageData(backgroundImageData);
    Resources::RemoveImageData(logoImageData);
    Resources::RemoveImageData(twitterLogoImageData);
    Resources::RemoveImageData(githubLogoImageData);
}
