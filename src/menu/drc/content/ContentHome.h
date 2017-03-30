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
#ifndef _CONTENT_HOME_H
#define _CONTENT_HOME_H

#include "gui/Gui.h"
#include "ContentHome.h"
#include "ContentTemplate.h"

class ContentHome : public ContentTemplate{
public:
    ContentHome();
    virtual ~ContentHome();

private:
    GuiImageData * logoImageData;
    GuiImage logoImage;
    GuiText welcomeHeadLineLabel;
    GuiText welcomeTextLabel;
    GuiText welcomeTextLabel2;
    GuiText welcomeTextLabel3;

    GuiImageData *twitterLogoImageData;
    GuiImage twitterLogoImage;

    GuiImageData *githubLogoImageData;
    GuiImage githubLogoImage;

    GuiText twitterLabel;


    GuiText URLLabel;
    GuiText URL2Label;

};

#endif //_CONTENT_HOME_H
