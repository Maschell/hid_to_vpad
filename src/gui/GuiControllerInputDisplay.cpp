/****************************************************************************
 * Copyright (C) 2016 Maschell
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
#include "GuiControllerInputDisplay.h"
#include "controller_patcher/ControllerPatcher.hpp"
#include "utils/StringTools.h"
#include "utils/logger.h"

GuiControllerInputDisplay::GuiControllerInputDisplay(UController_Type _controller_type): GuiFrame(441,324)
    , controller_type(_controller_type)
    , proImageData(Resources::GetImageData("buttons_pro_controller.png"))
    , proImage(proImageData)
    , gamepadImageData(Resources::GetImageData("buttons_gamepad_controller.png"))
    , gamepadImage(gamepadImageData)
    {
    std::string prefix;
    std::map<std::string,u32> * btn_list = NULL;
    if(controller_type == UController_Type_Gamepad){
        prefix = "buttons_gamepad_";
        append(&gamepadImage);
        btn_list = &gamepadButtonList;
        setScale(1.2f);
    }else{
        prefix = "buttons_pro_";
        append(&proImage);
        btn_list = &proButtonList;
    }
    if(btn_list == NULL){
        return;
    }
    typedef std::map<std::string, u32>::iterator it_type;
    for(it_type iterator = btn_list->begin(); iterator != btn_list->end(); iterator++) {
        ButtonConfig * btn_cfg = new ButtonConfig;
        if(btn_cfg == NULL){
             log_printf("Error on alloc ButtonConfig\n");
             continue;
        }
        memset(btn_cfg,0,sizeof(ButtonConfig));
        std::string * suffix = new std::string(iterator->first);
        btn_cfg->img.suffix = suffix;
        std::string filename = strfmt("%s%s.png",prefix.c_str(),btn_cfg->img.suffix->c_str());
        btn_cfg->img.imagedata = Resources::GetImageData(filename.c_str());
        if(btn_cfg->img.imagedata == NULL){
            log_printf("Error, was null!\n");
            delete btn_cfg;
            continue;
        }
        btn_cfg->img.image = new GuiImage(btn_cfg->img.imagedata);
        if(btn_cfg->img.image == NULL){
            log_printf("Error, was null! Removing ImageData\n");
            Resources::RemoveImageData(btn_cfg->img.imagedata);
            delete btn_cfg;
            continue;
        }
        btn_cfg->value = iterator->second;
        controllerConfig.buttons.push_back(btn_cfg);
        btn_cfg->img.image->setState(STATE_HIDDEN);
        append(btn_cfg->img.image);
    }
}

GuiControllerInputDisplay::~GuiControllerInputDisplay(){
    //Resources::RemoveImageData(proImageData); <-- will reduce lag. Will be deleted at the end anyway
    //Resources::RemoveImageData(gamepadImageData); <-- will reduce lag. Will be deleted at the end anyway
    for(std::vector<ButtonConfig *>::iterator it = controllerConfig.buttons.begin(); it != controllerConfig.buttons.end(); ++it) {
        ButtonConfig * btn_cfg = *it;
        if(btn_cfg != NULL){
            remove(btn_cfg->img.image);
            delete btn_cfg->img.image;
            delete btn_cfg->img.suffix;
            //Resources::RemoveImageData(btn_cfg->img.imagedata); <-- will reduce lag. Will be deleted at the end anyway
            delete btn_cfg;
        }
    }
}

void GuiControllerInputDisplay::process(){
    GuiFrame::process();
    u32 buttons_hold = 0;
    if(controller_type == UController_Type_Gamepad){
        //gamepadData.btns_h = 0;
        memset(&gamepadData,0,sizeof(VPADData));
        ControllerPatcher::setControllerDataFromHID(&gamepadData);
        buttons_hold = gamepadData.btns_h;

    }else{
        s32 chan = 0;
        if(controller_type == UController_Type_Pro1){ chan = 0;}
        if(controller_type == UController_Type_Pro2){ chan = 1;}
        if(controller_type == UController_Type_Pro3){ chan = 2;}
        if(controller_type == UController_Type_Pro4){ chan = 3;}
        s32 res = 0;
        if((res = ControllerPatcher::setProControllerDataFromHID((void *) &proData,chan,PRO_CONTROLLER_MODE_KPADDATA)) >= 0){
            buttons_hold = proData.pro.btns_h;
        }
    }

    for(std::vector<ButtonConfig *>::iterator it = controllerConfig.buttons.begin(); it != controllerConfig.buttons.end(); ++it) {
        ButtonConfig * btn_cfg = *it;
        if(btn_cfg != NULL){
            bool visible = false;
            if(btn_cfg->value & buttons_hold){
                visible = true;
            }
            if(btn_cfg->img.image != NULL){
                if(visible){
                    btn_cfg->img.image->clearState(STATE_HIDDEN);
                }else{
                    btn_cfg->img.image->setState(STATE_HIDDEN);
                }
            }
        }
    }
}
