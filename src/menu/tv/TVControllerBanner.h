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
#ifndef _TV_CONTROLLER_BANNER_H_
#define _TV_CONTROLLER_BANNER_H_
#include <vector>
#include "controller_patcher/ControllerPatcher.hpp"
#include <gui/Gui.h>
#include "TVButtonController.h"

class TVControllerBanner : public GuiFrame, public sigslot::has_slots<>
{
public:
    TVControllerBanner(s32 w, s32 h);
    virtual ~TVControllerBanner();

    void draw(CVideo *pVideo);
    void update(GuiController *c);
private:
    GuiImageData * background_imgdata;
    GuiImage background_img;

    TVButtonController proController1;
    TVButtonController proController2;
    TVButtonController proController3;
    TVButtonController proController4;
    TVButtonController gamepad;

    GuiImageData * homebutton_imgdata;
    GuiImage homebutton_img;

    GuiText exitHome;
    GuiFrame exitHomeFrame;

    GuiImageData * plusbutton_imgdata;
    GuiImage plusbutton_img;

    GuiText exitPlus;
    GuiFrame exitPlusFrame;

    GuiTrigger wpadTouchTrigger;

    UController_Type openNext;
    bool doOpenNext;

    void OnButtonClicked(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
};

#endif //_TV_CONTROLLER_BANNER_H_
