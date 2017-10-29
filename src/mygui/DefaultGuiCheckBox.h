/****************************************************************************
 * Copyright (C) 2017 Maschell
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
#ifndef DEFAULT_GUI_CHECKBOX_H_
#define DEFAULT_GUI_CHECKBOX_H_

#include <gui/GuiCheckBox.h>

//!A simple CheckBox
class DefaultGuiCheckBox : public GuiCheckBox{
	public:
		//!Constructor
		//!\param checked Checked
		DefaultGuiCheckBox(bool checked,f32 scale = 1.0f);
		//!Destructor
		virtual ~DefaultGuiCheckBox();
	protected:
       GuiImageData * checkbox_imgdata;
       GuiImage checkbox_img;

       GuiImageData * checkbox_selected_imgdata;
       GuiImage checkbox_selected_img;

       GuiImageData * highlighted_imgdata;
       GuiImage highlighted_img;
};

#endif
