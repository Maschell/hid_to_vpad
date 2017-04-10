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
#ifndef _MAIN_WINDOW_CONTENT_H_
#define _MAIN_WINDOW_CONTENT_H_

#include "gui/Gui.h"
#include "system/AsyncDeleter.h"
#include "MainWindowContent.h"
#include "content/ContentTemplate.h"

class MainWindowContent : public GuiFrame, public sigslot::has_slots<>{
public:
    MainWindowContent(s32 w, s32 h);
    virtual ~MainWindowContent();

    void SetScreen(ContentTemplate * new_content);
    void RemoveScreen();
    ContentTemplate * getContent();

private:
    s32 width, height;
    GuiImage bgImageColor;

    GuiImageData * footer_imgdata;
    GuiImage footer_img;

    GuiImageData * homebutton_imgdata;
    GuiImage homebutton_img;

    GuiText exitHome;
    GuiFrame exitHomeFrame;

    GuiImageData * plusbutton_imgdata;
    GuiImage plusbutton_img;

    GuiText exitPlus;
    GuiFrame exitPlusFrame;

    GuiFrame footerFrame;

    ContentTemplate * content = NULL;

    void OnCloseEffectFinish(GuiElement *element);
};

#endif //_MAIN_WINDOW_CONTENT_H_
