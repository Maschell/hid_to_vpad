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
#ifndef _CONTENT_TEMPLATE_H_
#define _CONTENT_TEMPLATE_H_

#include <gui/Gui.h>

class ContentTemplate : public GuiFrame, public sigslot::has_slots<>{

public:
    enum eContentNames{
        CONTENT_HOME,
        CONTENT_HELP,
        CONTENT_NETWORK_HELP,
        CONTENT_SETTINGS,
        CONTENT_ABOUT,
    };

    ContentTemplate() : GuiFrame(1280-450-2, 720)
    , bgImageColor(828, 720, (GX2Color){ 0, 0, 0, 0 })
    {
        bgImageColor.setImageColor((GX2Color){  248, 248, 249, 255 }, 0);
        bgImageColor.setImageColor((GX2Color){  248, 248, 249, 255 }, 1);
        bgImageColor.setImageColor((GX2Color){  248, 248, 249, 255 }, 2);
        bgImageColor.setImageColor((GX2Color){  248, 248, 249, 255 }, 3);
        append(&bgImageColor);
    }

    virtual ~ContentTemplate() {remove(&bgImageColor);}
protected:
    GuiImage bgImageColor;
};

#endif /* CONTENT_TEMPLATE_H_ */
