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
#ifndef _MENU_LIST_DRC_H_
#define _MENU_LIST_DRC_H_

#include "gui/Gui.h"
#include "gui/GuiDragListener.h"
#include "MainWindowContent.h"
#include "MenuListDRC.h"
#include "MenuElementController.h"
#include "MenuElementInfo.h"
#include "MenuSeperator.h"
#include "language/gettext.h"

class MenuListDRC : public GuiFrame, public sigslot::has_slots<>{
public:
    MenuListDRC(s32 w, s32 h,MainWindowContent * contentWindow);
    virtual ~MenuListDRC();

    MenuElement * getButtonElementByController(UController_Type controller_type);
    void OnButtonClicked(GuiButton * button, const GuiController *controller, GuiTrigger *trigger);
private:
    MainWindowContent * contentWindow;
    s32 width, height;
    GuiImage bgImageColor;
    MenuSeperator homeSeperator;
    MenuElementInfo elementHome;
    MenuSeperator controllerSeperator;
    MenuElementController elementGamePad;
    MenuElementController elementProController1;
    MenuElementController elementProController2;
    MenuElementController elementProController3;
    MenuElementController elementProController4;
    MenuSeperator otherSeperator;
    MenuElementInfo elementHelp;
    MenuElementInfo elementNetworkHelp;
    MenuElementInfo elementSettings;
    MenuElementInfo elementAbout;

    GuiTrigger buttonUpTrigger;
    GuiTrigger buttonDownTrigger;

    GuiTrigger touchTrigger;
    GuiButton DPADButtons;
    GuiDragListener VPADDrag;
    GuiTrigger wpadTouchTrigger;

    bool scroll_needs_update = false;
    bool dpad_selection_changed = false;
    f32 total_offset = 0.0f;

    f32 total_height_list = 0.0f;

    std::vector<GuiElement*> listElementsAll;
    std::vector<MenuElement*> listElementsButtons;

    s32 sizeListElementsAll = 0;
    s32 sizeListElementsButtons = 0;

    s32 selectedItem = 0;
    s32 lastSelectedItem = -1;

    void setState(s32 i, s32 c);
    void update(GuiController * c);

    void OnDrag(GuiDragListener * listener, const GuiController * controller, GuiTrigger * trigger,s32 dx,s32 dy);
    void addToTotalOffset(f32 added_offset);
    void calculateAndSetOffsets(std::vector<GuiElement*>& elements,f32 total_offset);
    void listAppend(GuiElement * listElement);
    void OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);

    void OnOpenEffectFinish(GuiElement *element);
};

#endif //_MENU_LIST_DRC_H_
