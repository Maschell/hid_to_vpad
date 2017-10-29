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
#include "DefaultGuiCheckBox.h"
/**
 * Constructor for the DefaultGuiCheckBox class.
 */

DefaultGuiCheckBox::DefaultGuiCheckBox(bool checked)
 : GuiCheckBox(checked)
 ,checkbox_imgdata(Resources::GetImageData("checkbox.png"))
 ,checkbox_img(checkbox_imgdata)
 ,checkbox_selected_imgdata(Resources::GetImageData("checkbox_selected.png"))
 ,checkbox_selected_img(checkbox_selected_imgdata)
 ,highlighted_imgdata(Resources::GetImageData("checkbox_highlighted.png"))
 ,highlighted_img(highlighted_imgdata)
{
    setSize(checkbox_img.getWidth(),checkbox_img.getHeight());
    this->setImageBackground(&checkbox_img);
    this->setImageSelected(&checkbox_selected_img);
    this->setImageHighlighted(&highlighted_img);
}

/**
 * Destructor for the GuiButton class.
 */
DefaultGuiCheckBox::~DefaultGuiCheckBox()
{
    Resources::RemoveImageData(checkbox_imgdata);
    Resources::RemoveImageData(checkbox_selected_imgdata);
    Resources::RemoveImageData(highlighted_imgdata);
}
