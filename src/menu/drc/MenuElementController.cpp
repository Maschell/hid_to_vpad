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
#include "MenuElementController.h"
#include "content/ContentController.h"
#include "controller_patcher/config/ConfigValues.hpp"
#include <vector>

/**
 * Constructor for the MenuElementController class.
 */

MenuElementController::MenuElementController(UController_Type controllertype)
 : MenuElement(450,120)
    ,controller_type(controllertype)
    ,controllerConnected(0)
    ,bg_imgdata(Resources::GetImageData("ItemBackgroundBig.png"))
    ,bg_img(bg_imgdata)
    ,highlighted_imgdata(Resources::GetImageData("selectorBig.png"))
    ,highlighted_img(highlighted_imgdata)
    ,icon_imgdata(Resources::GetImageData("itemIconBig.png"))
    ,icon_img(icon_imgdata)
    ,icon_highlighted_imgdata(Resources::GetImageData("itemIconBigSelected.png"))
    ,icon_highlighted_img(icon_highlighted_imgdata)
    ,proController_imgdata(Resources::GetImageData("proController.png"))
    ,proController_img(proController_imgdata)
    ,gamepad_imgdata(Resources::GetImageData("gamepad.png"))
    ,gamepad_img(gamepad_imgdata)
    ,controllerlabel("")
    ,notAttachedLabel(gettext("Nothing attached"))
    ,ledon_imgdata(Resources::GetImageData("ledon.png"))
    ,ledoff_imgdata(Resources::GetImageData("ledoff.png"))
{
    controllerlabel.setParent(&proController_img);
    controllerlabel.setAlignment(ALIGN_CENTER|ALIGN_BOTTOM);
    controllerlabel.setPosition(0,20);

    std::string text("");
    switch(controller_type){
        case UController_Type_Gamepad: { text = ""; break; }
        case UController_Type_Pro1: { text = "1"; break; }
        case UController_Type_Pro2: { text = "2"; break; }
        case UController_Type_Pro3: { text = "3"; break; }
        case UController_Type_Pro4: { text = "4"; break; }
        default: break;
    }
    controllerlabel.setColor(glm::vec4(0.95f,0.95f,0.95f,1.0f));
    controllerlabel.setText(text.c_str());

    notAttachedLabel.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    notAttachedLabel.setPosition(100+18,0);
    notAttachedLabel.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));
    notAttachedLabel.setParent(&icon_img);

    for(s32 i = 0;i<4;i++){
        ledImages.ledon[i] = new GuiImage(ledon_imgdata);
        ledImages.ledoff[i] = new GuiImage(ledoff_imgdata);

        ledImages.ledon[i]->setParent(&icon_img);
        ledImages.ledon[i]->setAlignment(ALIGN_BOTTOM);
        ledImages.ledoff[i]->setParent(&icon_img);
        ledImages.ledoff[i]->setAlignment(ALIGN_BOTTOM);
        ledImages.ledon[i]->setPosition(270+i*20,-10);
        ledImages.ledoff[i]->setPosition(270+i*20,-10);

        ledImages.ledon[i]->setAlpha(1.0f);
        ledImages.ledoff[i]->setAlpha(1.0f);
    }

    bg_img.setScale(height/bg_img.getHeight());
    highlighted_img.setScale(height/highlighted_img.getHeight());

    setImage(&bg_img);
    setIconOver(&highlighted_img);
    icon_img.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    icon_img.setPosition(20,0);
    icon_img.setParent(this);
    icon_highlighted_img.setParent(this);
    icon_highlighted_img.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    icon_highlighted_img.setPosition(20,0);
    proController_img.setParent(&icon_img);
    proController_img.setAlignment(ALIGN_CENTERED);
    gamepad_img.setParent(&icon_img);
    gamepad_img.setAlignment(ALIGN_CENTERED);

    controllerNameLabelVID.setParent(&icon_img);
    controllerNameLabelVID.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    controllerNameLabelVID.setPosition(118,20);
    controllerNameLabelVID.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    controllerNameLabelPID.setParent(&icon_img);
    controllerNameLabelPID.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    controllerNameLabelPID.setPosition(118,-10);
    controllerNameLabelPID.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    controllerNameLabelMiddle.setParent(&icon_img);
    controllerNameLabelMiddle.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    controllerNameLabelMiddle.setPosition(118,0);
    controllerNameLabelMiddle.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));
}

/**
 * Destructor for the MenuElementController class.
 */
MenuElementController::~MenuElementController()
{
    Resources::RemoveImageData(bg_imgdata);
    Resources::RemoveImageData(highlighted_imgdata);
    Resources::RemoveImageData(icon_imgdata);
    Resources::RemoveImageData(icon_highlighted_imgdata);
    Resources::RemoveImageData(proController_imgdata);
    Resources::RemoveImageData(gamepad_imgdata);

    Resources::RemoveImageData(ledon_imgdata);
    Resources::RemoveImageData(ledoff_imgdata);

    for(s32 i = 0;i<4;i++){
        if(ledImages.ledon[i]) delete ledImages.ledon[i];
        if(ledImages.ledoff[i]) delete ledImages.ledoff[i];
    }
}

void MenuElementController::drawControllerName(CVideo *v,u16 vid,u16 pid){
    std::string titleString = ControllerPatcher::getIdentifierByVIDPID(vid,pid);
    std::vector<std::string> result = CPStringTools::StringSplit(titleString, "\n");
    if(result.size() == 1){
        controllerNameLabelMiddle.setText(result.at(0).c_str());
        controllerNameLabelMiddle.draw(v);
    }else if(result.size() == 2){
        controllerNameLabelVID.setText(result.at(0).c_str());
        controllerNameLabelVID.draw(v);
        controllerNameLabelPID.setText(result.at(1).c_str());
        controllerNameLabelPID.draw(v);
    }
}

void MenuElementController::draw(CVideo *v){
	MenuElement::draw(v);
	if(!this->isVisible())
		return;

    std::string name = "";

    controllerConnected = 0;
    s32 found = ControllerPatcher::getActiveMappingSlot(getControllerType());
    if(found == -1){
        name = gettext("No device");
    }else{
        controllerConnected = 1;

        ControllerMappingPADInfo * info = ControllerPatcher::getControllerMappingInfo(getControllerType(),found);
        if(info != NULL){
            if(info->type == CM_Type_Controller){
                drawControllerName(v,info->vidpid.vid,info->vidpid.pid);
            }else if(info->type == CM_Type_RealController){
                name = gettext("Real Pro Controller");
            }else if(info->type == CM_Type_Mouse || info->type == CM_Type_Keyboard){
                name = gettext("Mouse / Keyboard");
            }
        }
    }
    notAttachedLabel.setText(name.c_str());
    notAttachedLabel.draw(v);

    if(!controllerConnected){
        icon_img.draw(v);
    }else{
        icon_highlighted_img.draw(v);
    }

    if(controller_type == UController_Type_Gamepad){
        gamepad_img.draw(v);
    }else{
        proController_img.draw(v);
        controllerlabel.draw(v);
    }
    bool pad_active = false;
    ControllerMappingPADInfo * info = ControllerPatcher::getControllerMappingInfo(getControllerType(),0);

    for(s32 i = 0;i<4;i++){
       if(info != NULL && (info->pad == i && info->active)){
            ledImages.ledon[i]->draw(v);
            pad_active = true;
        }else{
            ledImages.ledoff[i]->draw(v);
        }
    }

    float alpha = 0.3f;
    if(pad_active){
        alpha = 1.0f;
    }

    for(s32 i = 0;i<4;i++){
        ledImages.ledon[i]->setAlpha(alpha);
        ledImages.ledoff[i]->setAlpha(alpha);
    }
}

UController_Type MenuElementController::getControllerType(){
    return controller_type;
}

//If you call this function, please also delete the object. This class isn't taking care of it
ContentTemplate * MenuElementController::createNewContent(){
    return new ContentController(controller_type);
}

void MenuElementController::update(GuiController * c){
    MenuElement::update(c);
}

