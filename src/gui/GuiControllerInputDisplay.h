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
#ifndef GUI_CONTROLLER_INPUT_DISPLAY_H_
#define GUI_CONTROLLER_INPUT_DISPLAY_H_

#include "Gui.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/padscore_functions.h"
#include "controller_patcher/patcher/ControllerPatcherDefs.h"

#include <gctypes.h>
#include <map>
class GuiControllerInputDisplay : public GuiFrame{
    public:
        GuiControllerInputDisplay(UController_Type controller_type);
        ~GuiControllerInputDisplay();

        static void preLoadImages(){ // hacky...
            GuiControllerInputDisplay * instance = new GuiControllerInputDisplay(UController_Type_Gamepad);
            delete instance;
            instance = new GuiControllerInputDisplay(UController_Type_Pro1);
            delete instance;
        }
    private:
        UController_Type controller_type;

        GuiImageData * proImageData;
        GuiImage proImage;

        GuiImageData * gamepadImageData;
        GuiImage gamepadImage;

        typedef struct{
           std::string * suffix;
           GuiImageData * imagedata;
           GuiImage * image;
        }ImageConfig;

        typedef struct{
           ImageConfig img;
           u32 value;
        }ButtonConfig;

        typedef struct{
           std::vector<ButtonConfig *> buttons;
        }ControllerConfig;

        ControllerConfig controllerConfig;

        std::map<std::string, u32> proButtonList =   {{"a",             WPAD_PRO_BUTTON_A}
                                                    , {"b",             WPAD_PRO_BUTTON_B}
                                                    , {"x",             WPAD_PRO_BUTTON_X}
                                                    , {"y",             WPAD_PRO_BUTTON_Y}
                                                    , {"dpad_up",       WPAD_PRO_BUTTON_UP}
                                                    , {"dpad_down",     WPAD_PRO_BUTTON_DOWN}
                                                    , {"dpad_left",     WPAD_PRO_BUTTON_LEFT}
                                                    , {"dpad_right",    WPAD_PRO_BUTTON_RIGHT}
                                                    , {"home",          WPAD_PRO_BUTTON_HOME}
                                                    , {"plus",          WPAD_PRO_BUTTON_PLUS}
                                                    , {"minus",         WPAD_PRO_BUTTON_MINUS}
                                                    , {"l",             WPAD_PRO_TRIGGER_L}
                                                    , {"zl",            WPAD_PRO_TRIGGER_ZL}
                                                    , {"r",             WPAD_PRO_TRIGGER_R}
                                                    , {"zr",            WPAD_PRO_TRIGGER_ZR}
                                                    , {"l_stick",       WPAD_PRO_BUTTON_STICK_L}
                                                    , {"r_stick",       WPAD_PRO_BUTTON_STICK_R}
                                                    , {"r_stick_down",  WPAD_PRO_STICK_R_EMULATION_DOWN}
                                                    , {"r_stick_up",    WPAD_PRO_STICK_R_EMULATION_UP}
                                                    , {"r_stick_left",  WPAD_PRO_STICK_R_EMULATION_LEFT}
                                                    , {"r_stick_right", WPAD_PRO_STICK_R_EMULATION_RIGHT}
                                                    , {"l_stick_down",  WPAD_PRO_STICK_L_EMULATION_DOWN}
                                                    , {"l_stick_up",    WPAD_PRO_STICK_L_EMULATION_UP}
                                                    , {"l_stick_left",  WPAD_PRO_STICK_L_EMULATION_LEFT}
                                                    , {"l_stick_right", WPAD_PRO_STICK_L_EMULATION_RIGHT}
                                                    };

        std::map<std::string, u32> gamepadButtonList =  {{"a",          VPAD_BUTTON_A}
                                                    , {"b",             VPAD_BUTTON_B}
                                                    , {"x",             VPAD_BUTTON_X}
                                                    , {"y",             VPAD_BUTTON_Y}
                                                    , {"dpad_up",       VPAD_BUTTON_UP}
                                                    , {"dpad_down",     VPAD_BUTTON_DOWN}
                                                    , {"dpad_left",     VPAD_BUTTON_LEFT}
                                                    , {"dpad_right",    VPAD_BUTTON_RIGHT}
                                                    , {"home",          VPAD_BUTTON_HOME}
                                                    , {"plus",          VPAD_BUTTON_PLUS}
                                                    , {"minus",         VPAD_BUTTON_MINUS}
                                                    , {"l",             VPAD_BUTTON_L}
                                                    , {"zl",            VPAD_BUTTON_ZL}
                                                    , {"r",             VPAD_BUTTON_R}
                                                    , {"zr",            VPAD_BUTTON_ZR}
                                                    , {"l_stick",       VPAD_BUTTON_STICK_L}
                                                    , {"r_stick",       VPAD_BUTTON_STICK_R}
                                                    , {"r_stick_down",  VPAD_STICK_R_EMULATION_DOWN}
                                                    , {"r_stick_up",    VPAD_STICK_R_EMULATION_UP}
                                                    , {"r_stick_left",  VPAD_STICK_R_EMULATION_LEFT}
                                                    , {"r_stick_right", VPAD_STICK_R_EMULATION_RIGHT}
                                                    , {"l_stick_down",  VPAD_STICK_L_EMULATION_DOWN}
                                                    , {"l_stick_up",    VPAD_STICK_L_EMULATION_UP}
                                                    , {"l_stick_left",  VPAD_STICK_L_EMULATION_LEFT}
                                                    , {"l_stick_right", VPAD_STICK_L_EMULATION_RIGHT}
                                                    };

    KPADData proData;
    VPADData gamepadData;

    void process();

    bool hidebuttons;
};

#endif /* GUI_CONTROLLER_INPUT_DISPLAY_H_ */
