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
#include "MainWindowContent.h"

MainWindowContent::MainWindowContent(int w, int h)
    : GuiFrame(w, h)
    , width(w)
    , height(h)
    , bgImageColor(w, h, (GX2Color){ 0, 0, 0, 0 })
    , footer_imgdata(Resources::GetImageData("footer.png"))
    , footer_img(footer_imgdata)
    , homebutton_imgdata(Resources::GetImageData("HomeButtonIcon.png"))
    , homebutton_img(homebutton_imgdata)
    , exitHome("Exit to HBL  ")
    , plusbutton_imgdata(Resources::GetImageData("PlusButtonIcon.png"))
    , plusbutton_img(plusbutton_imgdata)
    , exitPlus("Apply Patches")
{
    bgImageColor.setImageColor((GX2Color){  248, 249, 248, 255 }, 0);
    bgImageColor.setImageColor((GX2Color){  248, 249, 248, 255 }, 1);
    bgImageColor.setImageColor((GX2Color){  248, 249, 248, 255 }, 2);
    bgImageColor.setImageColor((GX2Color){  248, 249, 248, 255 }, 3);


    homebutton_img.setPosition(-70,0);

    exitHome.setPosition(60,0);
    exitHome.setFontSize(35);
    exitHome.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    exitHomeFrame.setAlignment(ALIGN_CENTERED);
    exitHomeFrame.setPosition(170,20);
    exitHomeFrame.append(&homebutton_img);
    exitHomeFrame.append(&exitHome);
    exitHomeFrame.setScale(0.7f);

    plusbutton_img.setPosition(-70,0);

    exitPlus.setPosition(70,0);
    exitPlus.setFontSize(35);
    exitPlus.setColor(glm::vec4(0.2f,0.2f,0.2f,1.0f));

    exitPlusFrame.setAlignment(ALIGN_CENTERED);
    exitPlusFrame.setPosition(-260,20);
    exitPlusFrame.append(&plusbutton_img);
    exitPlusFrame.append(&exitPlus);
    exitPlusFrame.setScale(0.7f);


    footerFrame.append(&footer_img);
    footerFrame.append(&exitHomeFrame);
    footerFrame.append(&exitPlusFrame);

    footerFrame.setAlignment(ALIGN_BOTTOM);
    footer_img.setAlignment(ALIGN_BOTTOM);

    append(&footerFrame);
    append(&bgImageColor);
}

 ContentTemplate * MainWindowContent::getContent(){
    return content;
 }

void MainWindowContent::SetScreen(ContentTemplate * new_content){
    RemoveScreen();
    if(new_content){
        //while(content != NULL); //hopefully this shit don't kill us.

        content = new_content;
        append(content);

        bringToFront(&footerFrame);
    }
}

void MainWindowContent::RemoveScreen(){
    remove(content);
    delete content;
    content = NULL;
}

void MainWindowContent::OnCloseEffectFinish(GuiElement *element){

}

MainWindowContent::~MainWindowContent(){
    log_printf("~MainWindowContent()\n");
    remove(&footerFrame);
    remove(&bgImageColor);
    RemoveScreen();
}
