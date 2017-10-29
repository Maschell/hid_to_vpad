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
#include "ContentController.h"
#include "menu/drc/MainWindowDRC.h"
#include "menu/tv/MainWindowTV.h"

ContentController::ContentController(UController_Type controller_type_): ContentTemplate()
    , controller_type(controller_type_)
    , buttonATrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_A, true)
    , buttonXTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_X, true)
    , DPADButtons(0,0)
    , proinput(controller_type_)
    , connectedFrame(1280-450-2,720)
    , notConnectedFrame(1280-450-2,720)
    , notConnectedLabel(gettext("Currently no device is connected."))
    , notConnectedLabel2("")
    , not_connected_imgdata(Resources::GetImageData("not_connected.png"))
    , not_connected_img(not_connected_imgdata)
    , connectedLabel(gettext("Device connected. You can test it by pressing buttons."))
    , connectedLabel2(gettext("Press A to remap to a new controller."))
    , mouseConnectedLabel(gettext("Mouse connected. You can test it by pressing buttons."))
    , keyboardConnectedLabel(gettext("Keyboard connected. You can test it by pressing buttons."))
    , mouseConnectedLabel2(gettext("Press X to add a Keyboard."))
    , keyboardConnectedLabel2(gettext("Press X to add a Mouse."))
    , mouseConnectedLabel3(gettext("Keyboard connected."))
    , keyboardConnectedLabel3(gettext("Mouse connected."))
{
    std::string text("");
    switch(controller_type){
        case UController_Type_Gamepad: {text = UController_Type_Gamepad_Name;        break; }
        case UController_Type_Pro1: { text = UController_Type_Pro1_Name; break; }
        case UController_Type_Pro2: { text = UController_Type_Pro2_Name; break; }
        case UController_Type_Pro3: { text = UController_Type_Pro3_Name; break; }
        case UController_Type_Pro4: { text = UController_Type_Pro4_Name; break; }
        default: break;
    }
    notConnectedLabel2.setText(StringTools::strfmt("%s\"%s\".",gettext("Press A to map a controller to "),text.c_str()).c_str());
    notConnectedLabel2.setMaxWidth(800,GuiText::SCROLL_HORIZONTAL);

    headLine.setText(text.c_str());
    headLine.setFontSize(50);
    headLine.setAlignment(ALIGN_TOP_CENTER);
    headLine.setPosition(0,-110);
    headLine.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));

    connectedFrame.append(&headLine);
    notConnectedFrame.append(&headLine);

    notConnectedLabel.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    notConnectedLabel2.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    notConnectedLabel.setFontSize(28);
    notConnectedLabel2.setFontSize(33);


    notConnectedLabel.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    notConnectedLabel2.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    notConnectedLabel.setPosition(0,170);
    notConnectedLabel2.setPosition(0,100);
    not_connected_img.setPosition(0,50);
    proinput.setPosition(0,30);


    connectedLabel.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    connectedLabel.setFontSize(28);
    connectedLabel.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    connectedLabel.setPosition(0,170);

    connectedLabel2.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    connectedLabel2.setFontSize(35);
    connectedLabel2.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    connectedLabel2.setPosition(0,100);

    mouseConnectedLabel.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    mouseConnectedLabel.setFontSize(28);
    mouseConnectedLabel.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    mouseConnectedLabel.setPosition(0,180);

    mouseConnectedLabel2.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    mouseConnectedLabel2.setFontSize(28);
    mouseConnectedLabel2.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    mouseConnectedLabel2.setPosition(0,150);

    mouseConnectedLabel3.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    mouseConnectedLabel3.setFontSize(28);
    mouseConnectedLabel3.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    mouseConnectedLabel3.setPosition(0,150);

    keyboardConnectedLabel.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    keyboardConnectedLabel.setFontSize(28);
    keyboardConnectedLabel.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    keyboardConnectedLabel.setPosition(0,180);

    keyboardConnectedLabel2.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    keyboardConnectedLabel2.setFontSize(28);
    keyboardConnectedLabel2.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    keyboardConnectedLabel2.setPosition(0,150);

    keyboardConnectedLabel3.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    keyboardConnectedLabel3.setFontSize(28);
    keyboardConnectedLabel3.setAlignment(ALIGN_BOTTOM | ALIGN_CENTER);
    keyboardConnectedLabel3.setPosition(0,150);

    connectedFrame.append(&proinput);
    connectedFrame.append(&connectedLabel);
    connectedFrame.append(&connectedLabel2);
    connectedFrame.append(&mouseConnectedLabel);
    connectedFrame.append(&keyboardConnectedLabel);
    connectedFrame.append(&mouseConnectedLabel2);
    connectedFrame.append(&keyboardConnectedLabel2);
    connectedFrame.append(&mouseConnectedLabel3);
    connectedFrame.append(&keyboardConnectedLabel3);

    notConnectedFrame.append(&proinput);
    notConnectedFrame.append(&notConnectedLabel);
    notConnectedFrame.append(&notConnectedLabel2);
    notConnectedFrame.append(&not_connected_img);

    append(&notConnectedFrame);
    append(&connectedFrame);

    DPADButtons.setTrigger(&buttonATrigger);
    DPADButtons.setTrigger(&buttonXTrigger);
    DPADButtons.clicked.connect(this, &ContentController::OnDPADClick);
    append(&DPADButtons);

}

ContentController::~ContentController(){
    remove(&bgImageColor);
    remove(&headLine);
}

UController_Type ContentController::getControllerType(){
    return controller_type;
}

void ContentController::OnControllerOptionButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger){
    MainWindow * window = MainWindow::getInstance();
    if(window != NULL){
        window->OpenInputGetterMenu(getControllerType());
    }
}

void ContentController::OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger){
    if(trigger == &buttonATrigger || trigger == &buttonXTrigger){
        //! do not auto launch when wiimote is pointing to screen and presses A
        if((controller->chan & (GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5)) && controller->data.validPointer)
        {
            return;
        }
        if(trigger == &buttonXTrigger){
            if(!(isMouse() || isKeyboard())){
                return;
            }
        }else if(trigger == &buttonATrigger){
            //Reset settings for this controller.
            ControllerPatcher::resetControllerMapping(getControllerType());
        }
        OnControllerOptionButtonClick(button,controller,trigger);
    }
}
bool ContentController::isMouse(){
    ControllerMappingPADInfo * padInfo  = ControllerPatcher::getControllerMappingInfo(getControllerType(),0);
    if(padInfo != NULL){
        if(padInfo->type == CM_Type_Mouse){
            return true;
        }
    }
    return false;
}

bool ContentController::isKeyboard(){
    ControllerMappingPADInfo * padInfo  = ControllerPatcher::getControllerMappingInfo(getControllerType(),0);
    if(padInfo != NULL){
        if(padInfo->type == CM_Type_Keyboard){
            return true;
        }
    }
    return false;
}

void ContentController::process(){
    ContentTemplate::process();

    s32 connectedSlot = ControllerPatcher::getActiveMappingSlot(getControllerType());

    //TODO: maybe just do it once and not every time?
    if(connectedSlot != -1){
        notConnectedFrame.setState(STATE_DISABLE_INPUT);
        notConnectedFrame.setVisible(false);
        connectedFrame.clearState(STATE_DISABLE_INPUT);
        connectedFrame.setVisible(true);
        proinput.setAlpha(1.0f);
        proinput.clearState(STATE_DISABLE_INPUT);

        ControllerMappingPADInfo * padInfo  = ControllerPatcher::getControllerMappingInfo(getControllerType(),1);

        if(isMouse()){
            connectedLabel.setVisible(false);
            keyboardConnectedLabel.setVisible(false);
            keyboardConnectedLabel2.setVisible(false);
            keyboardConnectedLabel3.setVisible(false);

            mouseConnectedLabel.setVisible(true);

            if(padInfo != NULL && padInfo->active) { mouseConnectedLabel2.setVisible(false);mouseConnectedLabel3.setVisible(true); }
                                                else{ mouseConnectedLabel2.setVisible(true);mouseConnectedLabel3.setVisible(false);}
        }else if(isKeyboard()){
            connectedLabel.setVisible(false);
            mouseConnectedLabel.setVisible(false);
            mouseConnectedLabel2.setVisible(false);
            mouseConnectedLabel3.setVisible(false);

            keyboardConnectedLabel.setVisible(true);
            if(padInfo != NULL && padInfo->active) { keyboardConnectedLabel2.setVisible(false);keyboardConnectedLabel3.setVisible(true); }
                                                else{ keyboardConnectedLabel2.setVisible(true);keyboardConnectedLabel3.setVisible(false);}
        }else{
            mouseConnectedLabel.setVisible(false);
            mouseConnectedLabel2.setVisible(false);
            mouseConnectedLabel3.setVisible(false);
            keyboardConnectedLabel.setVisible(false);
            keyboardConnectedLabel2.setVisible(false);
            keyboardConnectedLabel3.setVisible(false);

            connectedLabel.setVisible(true);
        }
    }else{
        connectedFrame.setState(STATE_DISABLE_INPUT);
        connectedFrame.setVisible(false);
        notConnectedFrame.clearState(STATE_DISABLE_INPUT);
        notConnectedFrame.setVisible(true);
        proinput.setAlpha(0.2f);
        proinput.setState(STATE_DISABLE_INPUT);
    }
}
