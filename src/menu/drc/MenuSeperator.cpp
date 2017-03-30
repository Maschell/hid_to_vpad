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
#include "MenuSeperator.h"
/**
 * Constructor for the MenuSeperator class.
 */

MenuSeperator::MenuSeperator(std::string title):
    label(title.c_str())
    ,imgdata(Resources::GetImageData("seperator.png"))
    ,img(imgdata)
{
    setSize(imgdata->getWidth(),imgdata->getHeight());
    img.setParent(this);
    label.setAlignment(ALIGN_LEFT|ALIGN_MIDDLE);
    label.setPosition(25,0);
    label.setParent(this);

}

/**
 * Destructor for the MenuSeperator class.
 */
MenuSeperator::~MenuSeperator(){
    Resources::RemoveImageData(imgdata);
}

void MenuSeperator::draw(CVideo *v){
	if(!this->isVisible())
		return;

    img.draw(v);
    label.draw(v);
}

void MenuSeperator::update(GuiController * c){
    GuiElement::update(c);
}

