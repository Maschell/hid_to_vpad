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
#include "ContentNetworkHelp.h"

ContentNetworkHelp::ContentNetworkHelp(): ContentTemplate(){
    headLine.setText("Network Client");
    headLine.setFontSize(60);
    headLine.setAlignment(ALIGN_TOP_CENTER);
    headLine.setPosition(0,-110);
    headLine.setColor(glm::vec4(0.3f,0.3f,0.3f,1.0f));
    append(&headLine);

     //Copy paste from dimok's code. Thanks <3
    GuiText *text = NULL;

    f32 positionY = 200.0f;
    f32 positionX = 40.0f;
    f32 positionX2 = 250.0f;
    f32 positionX3 = 80.0f;

    int fontSize = 35;
    int fontSize2 = 24;
    glm::vec4 textColor = glm::vec4(0.3f,0.3f,0.3f,1.0f);

    text = new GuiText("What is the Network Client?", fontSize, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);

    positionY -= 35;
    text = new GuiText("It allows you to use your controller connected to your Computer with", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;


    text = new GuiText("HID to VPAD. This way for example XInput- and HID-Bluetooth-Devices", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;

    text = new GuiText("can be used. It connects to your console over your local network.", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;

     text = new GuiText("How do I use it?", fontSize, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;

    text = new GuiText("Enter the IP (upper right corner!) of your console and press connect.", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;

    text = new GuiText("If everything worked correctly, use the controller just like one", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;

    text = new GuiText("connected via USB. You can find a detailed guide here:", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;

    text = new GuiText("http://gbatemp.net/threads/hid-to-vpad.424127/", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;



    text = new GuiText("Is there any input lag?", fontSize, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;

    text = new GuiText("With a connection via Ethernet, you shouldn't notice any lag.", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;

    text = new GuiText("Wi-Fi may lead to some issues or lag, but it heavily depends on the", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;

    text = new GuiText("quality of your signal. If it's possible, connect everything via Ethernet.", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;

    text = new GuiText("Where can I get it?", fontSize, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);

    positionY -= 35;
    text = new GuiText("It's open source and you can find it here: ", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);

    positionY -= 25;
    text = new GuiText("https://github.com/QuarkTheAwesome/HIDtoVPADNetworkClient", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;

    /*
    text = new GuiText("Is controller XYZ supported", fontSize, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);

    positionY -= 35;
    text = new GuiText("In theory all XInput and HID devices should work this way. If the client", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;
    text = new GuiText("is recognizing your Controller, you only need a config file on the SD-Card", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 25;
    text = new GuiText("in your console, like a normal controller connected via USB.", fontSize2, textColor);
    text->setPosition(positionX, positionY);
    text->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
    helpText.push_back(text);
    append(text);
    positionY -= 35;*/
}

ContentNetworkHelp::~ContentNetworkHelp(){
    remove(&bgImageColor);
    remove(&headLine);

     for(u32 i = 0; i < helpText.size(); ++i){
        remove(helpText[i]);
        delete helpText[i];
    }
}
