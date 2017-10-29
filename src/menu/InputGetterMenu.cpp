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
#include <controller_patcher/ControllerPatcher.hpp>
#include <gui/GuiElement.h>
#include <utils/logger.h>
#include <language/gettext.h>
#include <dynamic_libs/padscore_functions.h>

#include "InputGetterMenu.h"
#include "Application.h"

CThread * InputGetterMenu::pThread = NULL;

InputGetterMenu::InputGetterMenu(UController_Type controller_type)
    : GuiFrame(0,0)
    , backgroundImageData(Resources::GetImageData("InputGetterBG.png"))
    , backgroundImage(backgroundImageData)
    , bgBlur(1280, 720, (GX2Color){0, 0, 0, 255})
    , infoController("", 70, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f))
    , infoText(gettext("Press any button on the USB-Controller you want to use."), 35, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
    , infoTextreturn(gettext("Press B to disable the mapping"), 55, glm::vec4(0.3f, 0.3f, 0.3f, 1.0f))
    , buttonBTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_B, true)
    , DPADButtons(0,0)
{
    setControllerType(controller_type);
    CVideo * video = Application::instance()->getVideo();
    width = video->getTvWidth();
    height = video->getTvHeight();
    gameLauncherMenuFrame = GuiFrame(width, height);
    std::string name;
    if(controller_type == UController_Type_Gamepad){
        name = UController_Type_Gamepad_Name;
    }else if(controller_type == UController_Type_Pro1){
        WPADDisconnect(0);
        name = UController_Type_Pro1_Name;
    }else if(controller_type == UController_Type_Pro2){
        WPADDisconnect(1);
        name = UController_Type_Pro2_Name;
    }else if(controller_type == UController_Type_Pro3){
        WPADDisconnect(2);
        name = UController_Type_Pro3_Name;
    }else if(controller_type == UController_Type_Pro4){
        WPADDisconnect(3);
        name = UController_Type_Pro4_Name;
    }

    infoController.setText(name.c_str());
    infoController.setAlignment(ALIGN_TOP|ALIGN_CENTER);
    infoController.setPosition(0,-155);
    infoText.setPosition(0,0);
    infoTextreturn.setPosition(0,-245);
    //infoTextreturn.setScale(2.0f);
    gameLauncherMenuFrame.append(&infoController);
    gameLauncherMenuFrame.append(&infoText);
    gameLauncherMenuFrame.append(&infoTextreturn);

    bgBlur.setAlpha(0.6f);
    append(&bgBlur);

    append(&backgroundImage);

    DPADButtons.setTrigger(&buttonBTrigger);

    DPADButtons.clicked.connect(this, &InputGetterMenu::OnDPADClick);
    gameLauncherMenuFrame.append(&DPADButtons);

    gameLauncherMenuFrame.append(&infoText);

    append(&gameLauncherMenuFrame);
    InputGetterMenu::GetInputsAsync(this);
}

InputGetterMenu::~InputGetterMenu(){
    Resources::RemoveImageData(backgroundImageData);
    //log_printf("Existing InputGetterMenu\n");
    if(InputGetterMenu::pThread != NULL){
        exitInputThread = 1;
        DCFlushRange((void*)&exitInputThread,sizeof(exitInputThread));
        //log_printf("exitInputThread = 1\n");
        delete InputGetterMenu::pThread;
        //log_printf("delete InputGetterMenu::pThread; done\n");
        InputGetterMenu::pThread = NULL;
    }
}

void InputGetterMenu::GetInputsAsync(InputGetterMenu * menu)
{
    InputGetterMenu::pThread = CThread::create(InputGetterMenu::GetInputs, (void*)menu, CThread::eAttributeAffCore2 | CThread::eAttributePinnedAff, 29);
    InputGetterMenu::pThread->resumeThread();
}


void InputGetterMenu::GetInputs(CThread *thread, void *arg){
    InputGetterMenu * args = (InputGetterMenu * )arg;

    s32 gotPress = 0;
    s32 inputsize =  gHIDMaxDevices;
    InputData * hiddata = (InputData * ) malloc(sizeof(InputData)*inputsize);
    memset(hiddata,0,sizeof(InputData)*inputsize);

    ControllerMappingPADInfo pad_result;
    memset(&pad_result,0,sizeof(ControllerMappingPADInfo));

    while(!gotPress){
        s32 foo = args->exitInputThread;
        foo +=1;
        if(args->exitInputThread)break;
        s32 result = ControllerPatcher::gettingInputAllDevices(hiddata,inputsize);
        if(result > 0){
            //log_printf("got %d results\n",result);
            for(s32 i =0;i<result;i++){
                for(s32 j = 0;j<HID_MAX_PADS_COUNT;j++){
                    //log_printf("check pad %d. %08X\n",j,hiddata[i].button_data[j].btn_h);
                    if(hiddata[i].button_data[j].btn_h != 0){
                        //log_printf("It pressed a buttons!\n",result);
                        pad_result.pad = j;
                        pad_result.vidpid.vid = hiddata[i].device_info.vidpid.vid;
                        pad_result.vidpid.pid = hiddata[i].device_info.vidpid.pid;
                        pad_result.active = 1;
                        pad_result.type = CM_Type_Controller;

                        if(pad_result.vidpid.vid == HID_KEYBOARD_VID && pad_result.vidpid.pid == HID_KEYBOARD_PID){
                            pad_result.type = CM_Type_Keyboard;
                        }else if(pad_result.vidpid.vid == HID_MOUSE_VID && pad_result.vidpid.pid == HID_MOUSE_PID){
                            pad_result.type = CM_Type_Mouse;
                        }

                        gotPress = 1;
                        log_printf("%04X %04X (PAD: %d) pressed a buttons %08X\n",hiddata[i].device_info.vidpid.vid,hiddata[i].device_info.vidpid.pid,j,hiddata[i].button_data[j].btn_h);
                        break;
                    }
                }
                if(gotPress)break;
            }
        }
    }
    if(gotPress){
        ControllerPatcher::addControllerMapping(args->getControllerType(),pad_result);
    }

    free(hiddata);
    if(!args->exitInputThread) args->OnQuitButtonClick(NULL,NULL,NULL);
}

void InputGetterMenu::OnDPADClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger){
	if(trigger == &buttonBTrigger){
	    if(pThread != NULL){
            exitInputThread = 1;
            delete InputGetterMenu::pThread;
            InputGetterMenu::pThread = NULL;
        }
        ControllerPatcher::resetControllerMapping(getControllerType());
        this->total_result = 1;
		OnQuitButtonClick(button,controller,trigger);
	}
}

UController_Type InputGetterMenu::getControllerType(){
    return this->controller_type;
}

void InputGetterMenu::setControllerType(UController_Type controller_type){
    this->controller_type = controller_type;
}

void InputGetterMenu::draw(CVideo *v)
{
    GuiFrame::draw(v);
}

void InputGetterMenu::update(GuiController *c)
{
    GuiFrame::update(c);
}
