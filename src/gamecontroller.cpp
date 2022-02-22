#include "consts.h"
#include "gamecontroller.h"

#include "timekeeper.h"
#include "scene.h"
#include "scenec.h"
#include "settings.h"
#include "uicontroller.h"
#include "inputmanager.h"
#include "windowmanager.h"

#include "shadercontroller.h"
#include "systemsinfomanager.h"
#include "cameracontroller.h"
#include "simulationmanager.h"
#include "systemmanager.h"
#include "currentgeneratorpoints.h"
#include "gridcontroller.h"

#include "messagelogger.h"

#include "boxcontroller.h"

namespace gamecontroller
{
    std::chrono::high_resolution_clock::time_point tid[3];

    float deltatime;
    int frames;
}

void gamecontroller::initialize()
{
    messagelogger::logmessage("gamecontroller", "starting initialization", warningseverity::information);

    settings::loadsettings(); //loader inn settings, must be first

    windowmanager::initialize();
    uicontroller::setupimgui();

    //stuff
    shadercontroller::initialize();
    inputmanager::initialize();
    cameracontroller::initialize();
    //

    simulationmanager::initialize();
    currentgeneratorpoints::initialize();

    systemmanager::initialize();

    gridcontroller::initialize();

    uielement::updateuiscale();

    scenec::initialize();

    messagelogger::logmessage("gamecontroller", "initialization complete", warningseverity::information);

    gameloop();
}

void gamecontroller::close()
{
    systemmanager::cleanup();
    currentgeneratorpoints::cleanup();

    windowmanager::close();
    uicontroller::destroyimgui();
}

void gamecontroller::gameloop()
{
    scenec::changeactivescene(sceneids::mainmenu);

    SDL_Event e;

    timekeeper::initialize();

    //THE loop
    while (!scenec::quit)
    {
        //--------- inputs og events ----------
        inputmanager::logcursorpos();//putt i refresh? neh

        //sdl events
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);

            inputmanager::processevent(e); //inputs, keys og mouse og bøttons
            windowmanager::processevent(e); //window events, i.e. resize, minimize, lost focus, etc

            if (e.type == SDL_QUIT)
                scenec::quitgame();
        }

        inputmanager::processheldkeys();



        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();//windowmanager::sdlwindow);
        ImGui::NewFrame();

        boxcontroller::updateboxes();

        if (windowmanager::windowhasfocus() || settings::getisetting(setting_enum::simulateinbg))
        {
            //if focus
            scenec::getactivescene().update();
        }

        render();

        uicontroller::updateuiscale();

        timekeeper::timekeeping();
    }
}


void gamecontroller::render()
{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, settings::getscreenx(), settings::getscreeny());
        //glEnable(GL_DEPTH_TEST);

        scenec::getactivescene().render();

        //ui
        uicontroller::render();


        SDL_GL_SwapWindow(windowmanager::sdlwindow);
}
