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
#ifndef _MAIN_WINDOW_DRC_H_
#define _MAIN_WINDOW_DRC_H_

#include <gui/Gui.h>
#include <gui/GuiConfigurationScreen.h>
#include <language/gettext.h>

#include "MenuListDRC.h"
#include "menu/MainWindow.h"
#include "MainWindowContent.h"

class MainWindowDRC : public GuiConfigurationScreen, public sigslot::has_slots<>{
public:
    MainWindowDRC(s32 w, s32 h);
    virtual ~MainWindowDRC();

    void clickListEntryByController(UController_Type controller_type);
private:
    s32 width, height;
    GuiImageData * windowSplitter_imgdata;
    GuiImage windowSplitter_img;
    MainWindowContent content;
    MenuListDRC menuList;
    GuiText versionText;
    GuiText ipAddress;

    void setState(s32 i, s32 c);
    void process();
};

#endif //_MAIN_WINDOW_DRC_H_
