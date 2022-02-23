#include "consts.h"
#include "windowmanager.h"

#include "messagelogger.h"


namespace windowmanager
{
    bool window_focus = true;
    SDL_Window* sdlwindow = nullptr;
    SDL_Renderer* sdlrenderer = nullptr;
    SDL_GLContext sdlglcontext = nullptr;
}

void windowmanager::reinitialize()
{
    //er dette trygt?

    close();
    initialize();
}

bool windowmanager::setupgamewindow()
{
    //starting sdl game window
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        messagelogger::logmessage("windowmanager", "Failed to initialize SDL", warningseverity::severe);
        return false;
    }

    windowmanager::sdlwindow = SDL_CreateWindow("megagalax", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings::getisetting(setting_enum::screen_x), settings::getisetting(setting_enum::screen_y), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!windowmanager::sdlwindow )
    {
        messagelogger::logmessage("windowmanager", "Unable to create SDL window", warningseverity::severe);
        return false;
    }

    return true;
}

bool windowmanager::setupopengl()
{
    //initializing opengl-sdl

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);//4.5 for compute shader

    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    windowmanager::sdlglcontext = SDL_GL_CreateContext(windowmanager::sdlwindow);

    SDL_GL_MakeCurrent(windowmanager::sdlwindow, windowmanager::sdlglcontext);

    if (!windowmanager::sdlglcontext)
    {
        messagelogger::logmessage("windowmanager", "Unable to create openGL context", warningseverity::severe);
        return false;
    }


    setvsync();

    // Init GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);

    //glDepthFunc(GL_LEQUAL);
    glDepthFunc(GL_LESS);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    //husk å reenable
    glEnable (GL_CULL_FACE); // cull face //disable for billboards??
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CW);

    return true;
}

void windowmanager::initialize()
{
    if (setupgamewindow())
        messagelogger::logmessage("windowmanager", "game window set up successfully", warningseverity::information);
    else
        messagelogger::logmessage("windowmanager", "error in game window setup", warningseverity::severe);

    if (setupopengl())
        messagelogger::logmessage("windowmanager", "opengl set up successfully", warningseverity::information);
    else
        messagelogger::logmessage("windowmanager", "error in opengl setup", warningseverity::severe);
}

void windowmanager::close()
{
    SDL_DestroyRenderer(windowmanager::sdlrenderer);
	SDL_DestroyWindow(windowmanager::sdlwindow);
	SDL_GL_DeleteContext(windowmanager::sdlglcontext);

	SDL_Quit();
}

float windowmanager::windowaspectratio()
{
    return (float)settings::getscreenx() / (float)settings::getscreeny();
}

bool windowmanager::windowhasfocus()
{
    return windowmanager::window_focus;
}

void windowmanager::resizewindow(int sx, int sy)
{
    //do stuff?
    settings::setsetting(setting_enum::screen_x, sx);
    settings::setsetting(setting_enum::screen_y, sy);
}

void windowmanager::setvsync(bool on)
{
    SDL_GL_SetSwapInterval(on);
}

void windowmanager::setvsync()
{
    SDL_GL_SetSwapInterval(settings::getisetting(setting_enum::vsync));
}

void windowmanager::processevent(SDL_Event& e)
{
    if (e.type == SDL_WINDOWEVENT)
    {
        switch (e.window.event)
        {
            case SDL_WINDOWEVENT_RESIZED:
                resizewindow(e.window.data1, e.window.data2); break;
            case SDL_WINDOWEVENT_MINIMIZED:
                windowmanager::window_focus = false; break;
            case SDL_WINDOWEVENT_RESTORED:
                windowmanager::window_focus = true; break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                windowmanager::window_focus = false; break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                windowmanager::window_focus = true; break;

        }
    }
}
