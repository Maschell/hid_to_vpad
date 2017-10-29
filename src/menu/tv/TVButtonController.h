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
#ifndef TV_BUTTON__CONTROLLER_H_
#define TV_BUTTON__CONTROLLER_H_

#include <controller_patcher/ControllerPatcherDefs.h>
#include <utils/StringTools.h>
#include <gui/Gui.h>
#include <language/gettext.h>

class TVButtonController : public GuiButton{
	public:
		//!Constructor
		//!\param checked Checked
		TVButtonController(UController_Type controllertype);
		//!Destructor
		virtual ~TVButtonController();

		UController_Type getControllerType();
	protected:
	    GuiImage blur;

	    UController_Type controller_type;
	    u8 controllerConnected;
        GuiImageData * bg_imgdata;
        GuiImage bg_img;

        GuiImageData * highlighted_imgdata;
        GuiImage highlighted_img;

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

#endif //TV_BUTTON__CONTROLLER_H_
