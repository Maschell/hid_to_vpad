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
#ifndef _MAIN_WINDOW_TV_H_
#define _MAIN_WINDOW_TV_H_

#include <vector>
#include <vector>
#include <queue>
#include <gui/Gui.h>
#include "mygui/GuiConfigurationScreen.h"

#include "TVControllerBanner.h"

class CVideo;

class MainWindowTV : public GuiConfigurationScreen{
public:
    MainWindowTV(s32 w, s32 h);
    virtual ~MainWindowTV();

    static TVControllerBanner * bannerInstance;

private:
    s32 width, height;

    GuiImageData *backgroundImageData;
    GuiImage backgroundImage;

    GuiImageData *logoImageData;
    GuiImage logoImage;

    GuiImageData *twitterLogoImageData;
    GuiImage twitterLogoImage;

    GuiImageData *githubLogoImageData;
    GuiImage githubLogoImage;

    GuiText twitterLabel;
    GuiText githubLabel;

    TVControllerBanner controllerConfigurationTV;
};

#endif //_MAIN_WINDOW_TV_H_
