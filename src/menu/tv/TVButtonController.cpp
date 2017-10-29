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
#include <utils/StringTools.h>
#include <controller_patcher/ControllerPatcherDefs.h>
#include <controller_patcher/ControllerPatcher.hpp>

#include "TVButtonController.h"

/**
 * Constructor for the TVButtonController class.
 */

TVButtonController::TVButtonController(UController_Type controllertype)
 : GuiButton(245,245)
    ,blur(245, 245, (GX2Color){ 255, 255, 255, 120 })
    ,controller_type(controllertype)
    ,controllerConnected(0)
    ,bg_imgdata(Resources::GetImageData("TVElementControllerBG.png"))
    ,bg_img(bg_imgdata)
    ,highlighted_imgdata(Resources::GetImageData("TVElementControllerBGSelected.png"))
    ,highlighted_img(highlighted_imgdata)
    ,proController_imgdata(Resources::GetImageData("proControllerBig.png"))
    ,proController_img(proController_imgdata)
    ,gamepad_imgdata(Resources::GetImageData("gamepadBig.png"))
    ,gamepad_img(gamepad_imgdata)
    ,controllerlabel("")
    ,notAttachedLabel(gettext("No device"))
    ,ledon_imgdata(Resources::GetImageData("ledon.png"))
    ,ledoff_imgdata(Resources::GetImageData("ledoff.png"))
{
    blur.setParent(this);

    controllerlabel.setParent(this);
    controllerlabel.setAlignment(ALIGN_CENTERED);
    controllerlabel.setPosition(0,-35);
    controllerlabel.setFontSize(33);

    std::string text("");
    switch(controller_type){
        case UController_Type_Gamepad: { text = ""; break; }
        case UController_Type_Pro1: { text = "1"; break; }
        case UController_Type_Pro2: { text = "2"; break; }
        case UController_Type_Pro3: { text = "3"; break; }
        case UController_Type_Pro4: { text = "4"; break; }
        default: break;
    }
    controllerlabel.setColor(glm::vec4(0.0f,0.0f,0.0f,1.0f));
    controllerlabel.setText(text.c_str());

    notAttachedLabel.setAlignment(ALIGN_CENTERED);
    notAttachedLabel.setPosition(0,40);
    notAttachedLabel.setFontSize(40);
    notAttachedLabel.setMaxWidth(220,GuiText::WRAP);
    notAttachedLabel.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));
    notAttachedLabel.setParent(this);

    for(s32 i = 0;i<4;i++){
        ledImages.ledon[i] = new GuiImage(ledon_imgdata);
        ledImages.ledoff[i] = new GuiImage(ledoff_imgdata);

        ledImages.ledon[i]->setParent(this);
        ledImages.ledon[i]->setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
        ledImages.ledon[i]->setScale(1.8f);
        ledImages.ledoff[i]->setParent(this);
        ledImages.ledoff[i]->setAlignment(ALIGN_BOTTOM|ALIGN_LEFT);
        ledImages.ledoff[i]->setScale(1.8f);
        ledImages.ledon[i]->setPosition(35+i*40,-4);
        ledImages.ledoff[i]->setPosition(35+i*40,-4);
        ledImages.ledon[i]->setAlpha(1.0f);
        ledImages.ledoff[i]->setAlpha(1.0f);

    }

    bg_img.setScale(height/bg_img.getHeight());
    highlighted_img.setScale(height/highlighted_img.getHeight());

    setImage(&bg_img);
    setIconOver(&highlighted_img);

    proController_img.setParent(this);
    proController_img.setAlignment(ALIGN_CENTERED);
    proController_img.setPosition(0,30);
    proController_img.setAlpha(0.4f);

    gamepad_img.setParent(this);
    gamepad_img.setAlignment(ALIGN_CENTERED);
    gamepad_img.setPosition(0,30);
    gamepad_img.setAlpha(0.4f);

    controllerNameLabelVID.setParent(this);
    controllerNameLabelVID.setAlignment(ALIGN_CENTERED);
    controllerNameLabelVID.setPosition(0,60);
    controllerNameLabelVID.setFontSize(40);
    controllerNameLabelVID.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    controllerNameLabelPID.setParent(this);
    controllerNameLabelPID.setAlignment(ALIGN_CENTERED);
    controllerNameLabelPID.setPosition(0,20);
    controllerNameLabelPID.setFontSize(40);
    controllerNameLabelPID.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    controllerNameLabelMiddle.setParent(this);
    controllerNameLabelMiddle.setAlignment(ALIGN_CENTERED);
    controllerNameLabelMiddle.setPosition(0,40);
    controllerNameLabelMiddle.setFontSize(40);
    controllerNameLabelMiddle.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));
}

/**
 * Destructor for the MenuElementController class.
 */
TVButtonController::~TVButtonController()
{
    Resources::RemoveImageData(bg_imgdata);
    Resources::RemoveImageData(highlighted_imgdata);
    Resources::RemoveImageData(proController_imgdata);
    Resources::RemoveImageData(gamepad_imgdata);

    Resources::RemoveImageData(ledon_imgdata);
    Resources::RemoveImageData(ledoff_imgdata);

    for(s32 i = 0;i<4;i++){
        if(ledImages.ledon[i]) delete ledImages.ledon[i];
        if(ledImages.ledoff[i]) delete ledImages.ledoff[i];
    }
}

void TVButtonController::drawControllerName(CVideo *v,u16 vid,u16 pid){
    std::string titleString = ControllerPatcher::getIdentifierByVIDPID(vid,pid);
    std::vector<std::string> result = StringTools::stringSplit(titleString, "\n");
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

void TVButtonController::draw(CVideo *v){
	GuiButton::draw(v);
	if(!this->isVisible())
		return;
    if(controller_type == UController_Type_Gamepad){
        gamepad_img.draw(v);
    }else{
        proController_img.draw(v);
        controllerlabel.draw(v);
    }

    blur.draw(v);

    std::string name ="";

    controllerConnected = 0;
    s32 foundSlot = ControllerPatcher::getActiveMappingSlot(getControllerType());

    if(foundSlot == -1){
        name = gettext("No device");
    }else{
        controllerConnected = 1;

        ControllerMappingPADInfo * info = ControllerPatcher::getControllerMappingInfo(getControllerType(),foundSlot);
        if(info !=NULL){
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

UController_Type TVButtonController::getControllerType(){
    return controller_type;
}

void TVButtonController::update(GuiController * c){
    GuiButton::update(c);
}

