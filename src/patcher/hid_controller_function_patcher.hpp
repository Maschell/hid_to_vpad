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

#ifndef _HID_CONTROLLER_FUNCTION_PATCHER_H
#define _HID_CONTROLLER_FUNCTION_PATCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <utils/function_patcher.h>

extern hooks_magic_t method_hooks_hid_controller[];
extern u32 method_hooks_size_hid_controller;
extern volatile u32 method_calls_hid_controller[];

#ifdef __cplusplus
}
#endif

#endif /* _HID_CONTROLLER_FUNCTION_PATCHER_H */
