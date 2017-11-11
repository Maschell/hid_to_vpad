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
#include "MenuListDRC.h"
#include <dynamic_libs/padscore_functions.h>
#include "content/ContentHome.h"
#include "content/ContentAbout.h"
#include "content/ContentHelp.h"
#include "content/ContentController.h"
#include <utils/logger.h>

MenuListDRC::MenuListDRC(s32 w, s32 h,MainWindowContent * _contentWindow)
    : GuiFrame(w, h)
    , contentWindow(_contentWindow)
    , width(w)
    , height(h)
    , bgImageColor(w, h, (GX2Color){ 0, 0, 0, 0 })
    , homeSeperator(gettext("Home"))
    , elementHome(gettext("Home"), "homeIcon.png", ContentTemplate::CONTENT_HOME)
    , controllerSeperator("Controller")
    , elementGamePad(UController_Type_Gamepad)
    , elementProController1(UController_Type_Pro1)
    , elementProController2(UController_Type_Pro2)
    , elementProController3(UController_Type_Pro3)
    , elementProController4(UController_Type_Pro4)
    , otherSeperator(gettext("Other"))
    , elementHelp(gettext("Help"),     "helpIcon.png", ContentTemplate::CONTENT_HELP)
    , elementNetworkHelp(  gettext("Network Client"),     "netbtn.png", ContentTemplate::CONTENT_NETWORK_HELP)
    , elementSettings(  gettext("Settings"),     "settingsbtn.png", ContentTemplate::CONTENT_SETTINGS)
    , elementAbout(gettext("About"),    "aboutIcon.png", ContentTemplate::CONTENT_ABOUT)
    , buttonUpTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_UP | GuiTrigger::STICK_L_UP, true)
    , buttonDownTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_DOWN | GuiTrigger::STICK_L_DOWN, true)
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , DPADButtons(0,0)
    , VPADDrag(w,h)
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
{
    bgImageColor.setImageColor((GX2Color){  255, 255, 255, 255 }, 0);
    bgImageColor.setImageColor((GX2Color){  255, 255, 255, 255 }, 1);
    bgImageColor.setImageColor((GX2Color){  255, 255, 255, 255 }, 2);
    bgImageColor.setImageColor((GX2Color){  255, 255, 255, 255 }, 3);

    append(&bgImageColor);

    listAppend(&homeSeperator);
    listAppend(&elementHome);
    listAppend(&controllerSeperator);
    listAppend(&elementGamePad);
    listAppend(&elementProController1);
    listAppend(&elementProController2);
    listAppend(&elementProController3);
    listAppend(&elementProController4);
    listAppend(&otherSeperator);
    listAppend(&elementNetworkHelp);
    listAppend(&elementHelp);
    listAppend(&elementSettings);
    listAppend(&elementAbout);

    sizeListElementsAll = listElementsAll.size();
    sizeListElementsButtons = listElementsButtons.size();

    DPADButtons.setTrigger(&buttonDownTrigger);
    DPADButtons.setTrigger(&buttonUpTrigger);
    DPADButtons.clicked.connect(this, &MenuListDRC::OnDPADClick);

    VPADDrag.setTrigger(&touchTrigger);
    VPADDrag.setTrigger(&wpadTouchTrigger);
    VPADDrag.dragged.connect(this, &MenuListDRC::OnDrag);

    append(&VPADDrag);
    append(&DPADButtons);
    scroll_needs_update = true;
    dpad_selection_changed = true;
}

MenuListDRC::~MenuListDRC(){
    remove(&bgImageColor);
    remove(&VPADDrag);
    remove(&DPADButtons);

    for (std::vector<GuiElement*>::iterator it = listElementsAll.begin() ; it != listElementsAll.end(); ++it){
        remove(*it);
    }

}

void MenuListDRC::setState(s32 i, s32 c){
    GuiFrame::setState(i,c);
}


void MenuListDRC::listAppend(GuiElement * listElement){
    listElement->setAlignment(ALIGN_TOP);

    //We need to save all drawn Element to get the scrolling working.
    listElementsAll.push_back(listElement);

    //Only add the buttons that can be clicked to a new list.
    MenuElement* realElement = dynamic_cast<MenuElement*>(listElement);
    if (realElement != NULL){
        listElementsButtons.push_back(realElement);
        realElement->released.connect(this, &MenuListDRC::OnButtonClicked);
        realElement->setdrawOverOnlyWhenSelected(true);
        realElement->setSelectable(false);
        realElement->setTrigger(&touchTrigger);
        realElement->setTrigger(&wpadTouchTrigger);
    }


    total_height_list += listElement->getHeight();
    append(listElement);
}

void MenuListDRC::OnDrag(GuiDragListener * listener, const GuiController * controller, GuiTrigger * trigger,s32 dx,s32 dy){
    addToTotalOffset((float)dy);
    scroll_needs_update = true;
}

void MenuListDRC::OnButtonClicked(GuiButton *button, const GuiController *controller, GuiTrigger *trigger){
    MenuElement * element = NULL;
    s32 i = 0;
    for (std::vector<MenuElement*>::iterator it = listElementsButtons.begin() ; it != listElementsButtons.end(); ++it){
        element = (*it);
        if(!element){continue; i++;}
        GuiButton* buttonElement = dynamic_cast<GuiButton*>(element);
        if(button == buttonElement){
            selectedItem = i;
            break;
        }
        i++;
    }
    dpad_selection_changed = true;
}

void MenuListDRC::OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger){
    if(trigger == &buttonUpTrigger || trigger == &buttonDownTrigger){
        if(trigger == &buttonUpTrigger){
                if(selectedItem > 0){
                    selectedItem--;
                }
        }else if(trigger == &buttonDownTrigger){
                if(selectedItem < sizeListElementsButtons-1){
                    selectedItem++;
                }
        }
        dpad_selection_changed = true;
    }
}

void MenuListDRC::addToTotalOffset(f32 added_offset){
    total_offset += added_offset;
    if(total_offset + getHeight() > total_height_list){
        total_offset = total_height_list - getHeight();
    }
    if(total_offset < 0) total_offset = 0.0f;
}
void MenuListDRC::update(GuiController * c){
    if(dpad_selection_changed && (lastSelectedItem != selectedItem)){
        lastSelectedItem = selectedItem;
        if(selectedItem >= 0 && (u32) selectedItem < listElementsButtons.size()){
            s32 i = 0;
            MenuElement * element = NULL;
            for (std::vector<MenuElement*>::iterator it = listElementsButtons.begin() ; it != listElementsButtons.end(); ++it){
                element = (*it);
                if(!element){continue; i++;}
                if(i == selectedItem){
                    element->setState(STATE_SELECTED);

                    //Open Window (ulgy)
                    if(contentWindow){
                        ContentTemplate * newContent = NULL;
                        newContent = element->createNewContent();
                        if(newContent){
                            newContent->setEffect(EFFECT_FADE, 10, 255);
                            newContent->setState(GuiElement::STATE_DISABLED);
                            newContent->effectFinished.connect(this, &MenuListDRC::OnOpenEffectFinish);
                            contentWindow->SetScreen(newContent);
                        }
                    }

                    // Ugly way of handling the auto scrolling
                    if(element && element->getOffsetY() > 0){
                        f32 offset =  element->getOffsetY() +40;
                        addToTotalOffset(-1*(offset));
                        scroll_needs_update = true;
                    }else if(-1*(element->getOffsetY() - element->getHeight()) > this->getHeight()){
                        if(i >=0 && (u32)i+1 < listElementsButtons.size()){
                            MenuElement *  nextElement = listElementsButtons[i+1];
                            if(nextElement != NULL){
                                addToTotalOffset(-1*(nextElement->getOffsetY()) - this->getHeight());
                            }
                        }else{
                            addToTotalOffset(-1*(element->getOffsetY() - element->getHeight()) - this->getHeight());
                        }
                        scroll_needs_update = true;
                    }
                }else{
                    element->clearState(STATE_SELECTED);
                }
                i++;
            }
        }
        dpad_selection_changed = false;
    }
     if(scroll_needs_update){
        calculateAndSetOffsets(listElementsAll,total_offset);
        scroll_needs_update = false;
    }
    GuiFrame::update(c);
}


void MenuListDRC::OnOpenEffectFinish(GuiElement *element){
    element->effectFinished.disconnect(this);
    element->clearState(GuiElement::STATE_DISABLED);
}

void MenuListDRC::calculateAndSetOffsets(std::vector<GuiElement*>& elements,f32 total_offset){
    f32 cur_offset = total_offset;
    for (std::vector<GuiElement*>::iterator it = elements.begin() ; it != elements.end(); ++it){
        (*it)->setPosition(0,cur_offset);
        cur_offset -= (*it)->getHeight();
    }
}

MenuElement * MenuListDRC::getButtonElementByController(UController_Type controller_type){
    switch(controller_type){
        case UController_Type_Gamepad:{
            return &elementGamePad;
        };
        case UController_Type_Pro1:{
            return &elementProController1;
        };
        case UController_Type_Pro2:{
            return &elementProController2;
        };
        case UController_Type_Pro3:{
            return &elementProController3;
        };
        case UController_Type_Pro4:{
            return &elementProController4;
        };
        default: return NULL;
    }
}
