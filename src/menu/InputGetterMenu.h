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
#ifndef _INPUTGETTERMENU_WINDOW_H_
#define _INPUTGETTERMENU_WINDOW_H_

#include "controller_patcher/ControllerPatcher.hpp"
#include "gui/Gui.h"
#include "system/CThread.h"

class InputGetterMenu : public GuiFrame, public sigslot::has_slots<>
{

public:
    InputGetterMenu(UController_Type controller_type);
    virtual ~InputGetterMenu();

    sigslot::signal3<GuiElement *,bool,UController_Type> inputGetterMenuQuit;

    void draw(CVideo *v);
    void update(GuiController *c);

private:
    void OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
    void OnQuitButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger)
    {
        inputGetterMenuQuit(this,true,getControllerType());
    }

    void GetInputsAsync(InputGetterMenu * menu);
    static void GetInputs(CThread *thread, void *arg);

    UController_Type getControllerType();
    void setControllerType(UController_Type controller_type);

    GuiFrame gameLauncherMenuFrame;
    GuiImageData * backgroundImageData;
    GuiImage backgroundImage;
    GuiImage bgBlur;
    GuiText infoController;
    GuiText infoText;
    GuiText infoTextreturn;
    GuiTrigger buttonBTrigger;
    GuiButton DPADButtons;
    UController_Type controller_type;
    volatile int total_result = 0;
    volatile int exitInputThread = 0;
    static CThread *pThread;
};

#endif //_INPUTGETTERMENU_WINDOW_H_
