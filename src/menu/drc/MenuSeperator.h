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
#ifndef MENU_SEPERATOR_H_
#define MENU_SEPERATOR_H_

#include "gui/GuiElement.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"

class MenuSeperator : public GuiElement
{
	public:
		//!Constructor
		//!\param checked Checked
		MenuSeperator(std::string title);
		//!Destructor
		virtual ~MenuSeperator();
	protected:
	    GuiText label;

	    GuiImageData * imgdata;
        GuiImage img;

        void draw(CVideo *video);
        void update(GuiController * c);
};

#endif
