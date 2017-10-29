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
#ifndef _CONTENT_NETWORK_HELP_H
#define _CONTENT_NETWORK_HELP_H

#include <gui/Gui.h>
#include <language/gettext.h>

#include "ContentTemplate.h"

class ContentNetworkHelp : public ContentTemplate{
public:
    ContentNetworkHelp();
    virtual ~ContentNetworkHelp();

private:
    GuiText headLine;
    std::vector<GuiText *> helpText;
};

#endif //_CONTENT_HELP_H
