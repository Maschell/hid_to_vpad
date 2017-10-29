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
#ifndef MENU_ELEMENT_INFO_H_
#define MENU_ELEMENT_INFO_H_

#include "MenuElement.h"
#include "content/ContentTemplate.h"
#include <language/gettext.h>

class MenuElementInfo : public MenuElement{
	public:
		//!Constructor
		//!\param checked Checked
		MenuElementInfo(std::string title,std::string iconPath,ContentTemplate::eContentNames _contentName);
		//!Destructor
		virtual ~MenuElementInfo();

		void setTitle(std::string newTitle);

		//If you call this function, please also delete the object. This class isn't taking care of it
        ContentTemplate * createNewContent();
	protected:
        GuiImageData * icon_imgdata;
        GuiImage icon_img;

        GuiText title;

        GuiImageData * bg_imgdata;
        GuiImage bg_img;

        GuiImageData * highlighted_imgdata;
        GuiImage highlighted_img;

        ContentTemplate::eContentNames contentName;

        void update(GuiController * c);
        void draw(CVideo *v);
};

#endif
