/****************************************************************************
 * Copyright (C) 2016 Maschell
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

#include "hid_controller_patcher.h"
#include "video/CursorDrawer.h"
#include "controller_patcher/cp_retain_vars.h"
#include "utils/logger.h"

DECL(void, GX2CopyColorBufferToScanBuffer, const GX2ColorBuffer *colorBuffer, s32 scan_target){
    if(gHIDCurrentDevice & gHID_LIST_MOUSE && gHID_Mouse_Mode == HID_MOUSE_MODE_TOUCH) {
        CursorDrawer::draw(gHID_Mouse.pad_data[0].data[0].X, gHID_Mouse.pad_data[0].data[0].Y);
    }
    real_GX2CopyColorBufferToScanBuffer(colorBuffer,scan_target);
}

DECL(void, _Exit, void){
    CursorDrawer::destroyInstance();
    real__Exit();
}

DECL(int, VPADRead, int chan, VPADData *buffer, u32 buffer_size, s32 *error) {
    int result = real_VPADRead(chan, buffer, buffer_size, error);

    if(gHIDAttached){
        setControllerDataFromHID(buffer,HID_ALL_CONNECTED_DEVICES);
    }

    if(gButtonRemappingConfigDone){
        buttonRemapping(buffer);
        if (HID_DEBUG) printButtons(buffer);
    }

    return result;
}

hooks_magic_t method_hooks_hid_controller[] __attribute__((section(".data"))) = {
    MAKE_MAGIC(VPADRead,                            LIB_VPAD,       STATIC_FUNCTION),
    MAKE_MAGIC(GX2CopyColorBufferToScanBuffer,      LIB_GX2,        STATIC_FUNCTION),
    MAKE_MAGIC(_Exit,                               LIB_CORE_INIT,  STATIC_FUNCTION),
};

u32 method_hooks_size_hid_controller __attribute__((section(".data"))) = sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t);

//! buffer to store our instructions needed for our replacements
volatile unsigned int method_calls_hid_controller[sizeof(method_hooks_hid_controller) / sizeof(hooks_magic_t) * FUNCTION_PATCHER_METHOD_STORE_SIZE] __attribute__((section(".data")));

