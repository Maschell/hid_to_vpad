/****************************************************************************
 * Copyright (C) 2016 Maschell
 * based on GuiButton by dimok
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
#include "GuiDragListener.h"
#include "GuiController.h"
#include "utils/logger.h"

/**
 * Constructor for the GuiDragListener class.
 */

GuiDragListener::GuiDragListener(f32 w,f32 h){
    width = w;
    height = h;
	for(int i = 0; i < iMaxGuiTriggers; i++)
	{
		trigger[i] = NULL;
	}
}

/**
 * Destructor for the GuiDragListener class.
 */
GuiDragListener::~GuiDragListener(){
}

void GuiDragListener::setState(int i, int c){
    GuiElement::setState(i,c);
}

void GuiDragListener::setTrigger(GuiTrigger * t, int idx){
    if(idx >= 0 && idx < iMaxGuiTriggers)
    {
        trigger[idx] = t;
    }
    else
    {
        for(int i = 0; i < iMaxGuiTriggers; i++)
        {
            if(!trigger[i])
            {
                trigger[i] = t;
                break;
            }
        }
    }
}

void GuiDragListener::update(GuiController * c){
	if(!c || isStateSet(STATE_DISABLED|STATE_HIDDEN|STATE_DISABLE_INPUT, c->chan))
		return;
	else if(parentElement && (parentElement->isStateSet(STATE_DISABLED|STATE_HIDDEN|STATE_DISABLE_INPUT, c->chan)))
		return;

    for(int i = 0; i < iMaxGuiTriggers; i++){
        if(!trigger[i]){
            continue;
        }

        bool isHeld = trigger[i]->held(c);


        if(isHeld && this->isInside(c->data.x, c->data.y)){
            int dx = c->data.x - c->lastData.x;
            int dy = c->data.y - c->lastData.y;

            if(dx == 0 && dy == 0) continue;

            dragged(this, c, trigger[i],dx,dy);
        }
    }
}
