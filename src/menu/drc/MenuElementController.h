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
#ifndef MENU_ELEMENT_CONTROLLER_H_
#define MENU_ELEMENT_CONTROLLER_H_

#include "MenuElement.h"
#include "controller_patcher/patcher/ControllerPatcherDefs.h"
#include "controller_patcher/utils/CPRetainVars.hpp"
#include "utils/StringTools.h"
#include "menu/ControllerDefs.h"

class MenuElementController : public MenuElement{
	public:
		//!Constructor
		//!\param checked Checked
		MenuElementController(UController_Type controllertype);
		//!Destructor
		virtual ~MenuElementController();

		UController_Type getControllerType();

		 //If you call this function, please also delete the object. This class isn't taking care of it
        ContentTemplate * createNewContent();
	protected:
	    UController_Type controller_type;
	    u8 controllerConnected;
        GuiImageData * bg_imgdata;
        GuiImage bg_img;

        GuiImageData * highlighted_imgdata;
        GuiImage highlighted_img;

        GuiImageData * icon_imgdata;
        GuiImage icon_img;

        GuiImageData * icon_highlighted_imgdata;
        GuiImage icon_highlighted_img;

        GuiImageData * proController_imgdata;
        GuiImage proController_img;

        GuiImageData * gamepad_imgdata;
        GuiImage gamepad_img;

        GuiText controllerlabel;
        GuiText notAttachedLabel;
        GuiText controllerNameLabelPID;
        GuiText controllerNameLabelVID;
        GuiText controllerNameLabelMiddle;

        GuiImageData * ledon_imgdata;
        GuiImageData * ledoff_imgdata;

        typedef struct{
            GuiImage * ledon[4];
            GuiImage * ledoff[4];
        }LEDImages;

        LEDImages ledImages;

        void drawControllerName(CVideo *v,u16 vid,u16 pid);

        void draw(CVideo *v);
        void update(GuiController * c);
};

#endif
