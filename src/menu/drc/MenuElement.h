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
#ifndef MENU_ELEMENT_H_
#define MENU_ELEMENT_H_

#include "gui/GuiButton.h"
#include "content/ContentTemplate.h"
#include "language/gettext.h"

class MenuElement : public GuiButton
{
	public:
		//!Constructor
		//!\param checked Checked
		MenuElement(f32 width,f32 height);
		//!Destructor
		virtual ~MenuElement();
		virtual ContentTemplate * createNewContent() = 0;
	protected:
        void update(GuiController * c);
};

#endif
