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
#include "TVControllerBanner.h"
#include "common/common.h"
#include "Application.h"
#include "fs/DirList.h"
#include "fs/fs_utils.h"
#include "system/AsyncDeleter.h"
#include "controller_patcher/utils/CPRetainVars.hpp"
#include "menu/drc/MenuListDRC.h"
#include "menu/drc/MainWindowDRC.h"
#include "menu/drc/content/ContentController.h"
#include "utils/StringTools.h"


TVControllerBanner::TVControllerBanner(int w, int h)
    : GuiFrame(w, h)
    , background_imgdata(Resources::GetImageData("MainWindowTVBackgroundBanner.png"))
    , background_img(background_imgdata)
    , proController1(UController_Type_Pro1)
    , proController2(UController_Type_Pro2)
    , proController3(UController_Type_Pro3)
    , proController4(UController_Type_Pro4)
    , gamepad(UController_Type_Gamepad)
    , homebutton_imgdata(Resources::GetImageData("HomeButtonIcon.png"))
    , homebutton_img(homebutton_imgdata)
    , exitHome("Press       to return to HBL")
    , plusbutton_imgdata(Resources::GetImageData("PlusButtonIcon.png"))
    , plusbutton_img(plusbutton_imgdata)
    , exitPlus("Press       to apply patches")
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
{
    append(&background_img);
    proController1.setAlignment(ALIGN_MIDDLE|ALIGN_LEFT);
    proController1.setPosition(8,0);
    proController1.setRumble(true);
    proController1.setTrigger(&wpadTouchTrigger);
    proController1.released.connect(this, &TVControllerBanner::OnButtonClicked);

    proController2.setAlignment(ALIGN_MIDDLE|ALIGN_LEFT);
    proController2.setPosition(8 + (proController1.getWidth() + 10) * 1,0);
    proController2.setTrigger(&wpadTouchTrigger);
    proController2.released.connect(this, &TVControllerBanner::OnButtonClicked);

    gamepad.setAlignment(ALIGN_MIDDLE|ALIGN_LEFT);
    gamepad.setPosition(8 + (proController1.getWidth() + 10) * 2,0);
    gamepad.setTrigger(&wpadTouchTrigger);
    gamepad.released.connect(this, &TVControllerBanner::OnButtonClicked);

    proController3.setAlignment(ALIGN_MIDDLE|ALIGN_LEFT);
    proController3.setPosition(8 + (proController1.getWidth() + 10) * 3,0);
    proController3.setTrigger(&wpadTouchTrigger);
    proController3.released.connect(this, &TVControllerBanner::OnButtonClicked);

    proController4.setAlignment(ALIGN_MIDDLE|ALIGN_LEFT);
    proController4.setPosition(8 + (proController1.getWidth() + 10) * 4,0);
    proController4.setTrigger(&wpadTouchTrigger);
    proController4.released.connect(this, &TVControllerBanner::OnButtonClicked);

    homebutton_img.setPosition(-88,-3);

    exitHome.setPosition(0,0);
    exitHome.setFontSize(35);
    exitHome.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    exitHomeFrame.setAlignment(ALIGN_CENTERED);
    exitHomeFrame.setPosition(240,-155);
    exitHomeFrame.append(&homebutton_img);
    exitHomeFrame.append(&exitHome);

    plusbutton_img.setPosition(-88,-3);

    exitPlus.setPosition(0,0);
    exitPlus.setFontSize(35);
    exitPlus.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    exitPlusFrame.setAlignment(ALIGN_CENTERED);
    exitPlusFrame.setPosition(-240,-155);
    exitPlusFrame.append(&plusbutton_img);
    exitPlusFrame.append(&exitPlus);

    append(&proController1);
    append(&proController2);
    append(&proController3);
    append(&proController4);
    append(&gamepad);

    append(&exitHomeFrame);
    append(&exitPlusFrame);
}

TVControllerBanner::~TVControllerBanner(){
    Resources::RemoveImageData(background_imgdata);
    Resources::RemoveImageData(homebutton_imgdata);
    Resources::RemoveImageData(plusbutton_imgdata);
    remove(&background_img);
    remove(&proController1);
    remove(&proController2);
    remove(&proController3);
    remove(&proController4);
    remove(&homebutton_img);
}

void TVControllerBanner::OnButtonClicked(GuiButton *button, const GuiController *controller, GuiTrigger *trigger){
    UController_Type controller_type = UController_Type_Gamepad;

    if(button == &gamepad){
        controller_type = UController_Type_Gamepad;
    }else if(button == &proController1){
        controller_type = UController_Type_Pro1;
    }else if(button == &proController2){
        controller_type = UController_Type_Pro2;
    }else if(button == &proController3){
        controller_type = UController_Type_Pro3;
    }else if(button == &proController4){
        controller_type = UController_Type_Pro4;
    }else{
        return;
    }

    MainWindow * window = MainWindow::getInstance();

    if(window != NULL){
         window->openPageForController(controller_type);

         window->OpenInputGetterMenu(controller_type);
    }
}

void TVControllerBanner::draw(CVideo *pVideo)
{
    GuiFrame::draw(pVideo);
}

void TVControllerBanner::update(GuiController *c){
    GuiFrame::update(c);
}
