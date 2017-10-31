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
#ifndef _CONTENT_CONTROLLER_H
#define _CONTENT_CONTROLLER_H

#include <gui/Gui.h>
#include <language/gettext.h>

#include "ContentHome.h"
#include "ContentTemplate.h"
#include "menu/drc/MenuElementController.h"
#include "menu/InputGetterMenu.h"
#include "custom/gui/GuiControllerInputDisplay.h"

class ContentController : public ContentTemplate{
public:
    ContentController(UController_Type controller_type);
    virtual ~ContentController();

    void OpenInputGetterMenu();
    UController_Type getControllerType();
private:
    UController_Type controller_type;
    GuiTrigger buttonATrigger;
    GuiTrigger buttonXTrigger;
    GuiTrigger wpadTouchTrigger;
    GuiButton DPADButtons;
    GuiText headLine;

    GuiControllerInputDisplay  proinput;

    GuiFrame connectedFrame;
    GuiFrame notConnectedFrame;
    GuiText notConnectedLabel;
    GuiText notConnectedLabel2;
    GuiText notConnectedLabel3;

    GuiImageData * not_connected_imgdata;
    GuiImage not_connected_img;

    GuiText connectedLabel;
    GuiText connectedLabel2;
    GuiText mouseConnectedLabel;
    GuiText keyboardConnectedLabel;

    GuiText mouseConnectedLabel2;
    GuiText keyboardConnectedLabel2;

    GuiText mouseConnectedLabel3;
    GuiText keyboardConnectedLabel3;

    void OnControllerOptionButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
    void OnInputGetterMenuFinish(GuiElement *element, bool result,UController_Type controller_type);
    void OnOpenEffectFinish(GuiElement *element);
    void OnCloseEffectFinish(GuiElement *element);
    void OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
    void process();

    bool isMouse();
    bool isKeyboard();
};
#endif //_CONTENT_CONTROLLER_H
