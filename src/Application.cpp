/****************************************************************************
 * Copyright (C) 2015 Dimok
 * Modified by Maschell, 2016,2017 for HID to VPAD
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
#include "Application.h"
#include "common/common.h"
#include "dynamic_libs/os_functions.h"
#include "gui/FreeTypeGX.h"
#include "gui/VPadController.h"
#include "gui/WPadController.h"
#include "resources/Resources.h"
#include "sounds/SoundHandler.hpp"
#include "utils/logger.h"

Application *Application::applicationInstance = NULL;
bool Application::exitApplication = false;

Application::Application()
    : CThread(CThread::eAttributeAffCore1 | CThread::eAttributePinnedAff, 0, 0x20000)
    , bgMusic(NULL)
    , video(NULL)
    , mainWindow(NULL)
    , exitCode(EXIT_RELAUNCH_ON_LOAD)
{
    controller[0] = new VPadController(GuiTrigger::CHANNEL_1);
    controller[1] = new WPadController(GuiTrigger::CHANNEL_2);
    controller[2] = new WPadController(GuiTrigger::CHANNEL_3);
    controller[3] = new WPadController(GuiTrigger::CHANNEL_4);
    controller[4] = new WPadController(GuiTrigger::CHANNEL_5);

    //! create bgMusic
    bgMusic = new GuiSound(Resources::GetFile("bgMusic.mp3"), Resources::GetFileSize("bgMusic.mp3"));

    exitApplication = false;
}

Application::~Application(){
    log_printf("Destroy music\n");

    delete bgMusic;

    log_printf("Destroy controller\n");

    for(int i = 0; i < 5; i++)
        delete controller[i];

    //We may have to handle Asyncdelete in the Destructors.
    log_printf("Destroy async deleter\n");
    do{
        log_printf("Triggering AsyncDeleter\n");
        AsyncDeleter::triggerDeleteProcess();
        while(!AsyncDeleter::realListEmpty()){
            usleep(1000);
        }
    }while(!AsyncDeleter::deleteListEmpty());
    AsyncDeleter::destroyInstance();

    log_printf("Clear resources\n");
    Resources::Clear();

    log_printf("Stop sound handler\n");
    SoundHandler::DestroyInstance();
}

int Application::exec(){
    //! start main GX2 thread
    resumeThread();
    //! now wait for thread to finish
    shutdownThread();

    return exitCode;
}

void Application::fadeOut(){
    GuiImage fadeOut(video->getTvWidth(), video->getTvHeight(), (GX2Color){ 0, 0, 0, 255 });

    for(int i = 0; i < 255; i += 10)
    {
        if(i > 255)
            i = 255;

        fadeOut.setAlpha(i / 255.0f);

        //! start rendering DRC
        video->prepareDrcRendering();
        mainWindow->drawDrc(video);

        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_ALWAYS);
        fadeOut.draw(video);
        GX2SetDepthOnlyControl(GX2_ENABLE, GX2_ENABLE, GX2_COMPARE_LEQUAL);

        video->drcDrawDone();

        //! start rendering TV
        video->prepareTvRendering();

        mainWindow->drawTv(video);

        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_ALWAYS);
        fadeOut.draw(video);
        GX2SetDepthOnlyControl(GX2_ENABLE, GX2_ENABLE, GX2_COMPARE_LEQUAL);

        video->tvDrawDone();

        //! as last point update the effects as it can drop elements
        mainWindow->updateEffects();

        video->waitForVSync();
    }

    //! one last cleared black screen
    video->prepareDrcRendering();
    video->drcDrawDone();
    video->prepareTvRendering();
    video->tvDrawDone();
    video->waitForVSync();
    video->tvEnable(false);
    video->drcEnable(false);
}

void Application::executeThread(void){
    log_printf("Initialize video\n");
    video = new CVideo(GX2_TV_SCAN_MODE_720P, GX2_DRC_SINGLE);

    log_printf("Video size %i x %i\n", video->getTvWidth(), video->getTvHeight());

    //! setup default Font
    log_printf("Initialize main font system\n");
    FreeTypeGX *fontSystem = new FreeTypeGX(Resources::GetFile("font.ttf"), Resources::GetFileSize("font.ttf"), true);
    GuiText::setPresetFont(fontSystem);

    log_printf("Initialize main window\n");

    mainWindow = MainWindow::getInstance(video->getTvWidth(), video->getTvHeight());

    bgMusic->SetLoop(true);
    bgMusic->Play();
    bgMusic->SetVolume(50);

    log_printf("Entering main loop\n");

    //! main GX2 loop (60 Hz cycle with max priority on core 1)
    while(!exitApplication)
    {
        //! Read out inputs
        for(int i = 0; i < 5; i++)
        {
            if(controller[i]->update(video->getTvWidth(), video->getTvHeight()) == false)
                continue;



            if(controller[i]->data.buttons_d & VPAD_BUTTON_PLUS){
                exitCode = APPLICATION_CLOSE_APPLY;
                exitApplication = true;
            }

            if(controller[i]->data.buttons_d & VPAD_BUTTON_HOME){
                exitCode = APPLICATION_CLOSE_MIIMAKER;
                exitApplication = true;
            }


            //! update controller states
            mainWindow->update(controller[i]);
        }
        mainWindow->process();

        //! start rendering DRC
        video->prepareDrcRendering();
        mainWindow->drawDrc(video);
        video->drcDrawDone();

        //! start rendering TV
        video->prepareTvRendering();
        mainWindow->drawTv(video);
        video->tvDrawDone();

        //! enable screen after first frame render
        if(video->getFrameCount() == 0) {
            video->tvEnable(true);
            video->drcEnable(true);
        }

        //! as last point update the effects as it can drop elements
        mainWindow->updateEffects();

        video->waitForVSync();

        //! transfer elements to real delete list here after all processes are finished
        //! the elements are transfered to another list to delete the elements in a separate thread
        //! and avoid blocking the GUI thread
        AsyncDeleter::triggerDeleteProcess();
    }

    fadeOut();

    MainWindow::destroyInstance();
    delete fontSystem;
    delete video;
}
