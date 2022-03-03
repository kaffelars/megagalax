#include "consts.h"
#include "uicontroller.h"
#include "settings.h"

namespace uicontroller
{
    int activefont = 0;
    bool updatescale = true;

    namespace
    {
        ImGuiIO* io = nullptr;
        ImFont* font[2] = {nullptr, nullptr};//
        glm::vec2 curwindowsize = glm::vec2(0.0f);
    }
}



void uicontroller::render()
{
    //lines
    //linedrawer::render(); //trenger camera


    ImGui::Render();
    glViewport(0, 0, (int)(io->DisplaySize.x), (int)(io->DisplaySize.y));
    //glClearColor(0.0, 0.0, 0.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    uielement::resetcounter();
}

bool uicontroller::uimouse()
{
    return io->WantCaptureMouse;
}


float uicontroller::xpos(float ex)
{
    if (ex < 0.0f) ex = settings::getscreenx() + ex;
    //ex *= settings::getuiscale();
    return ex;
}

float uicontroller::ypos(float ey)
{
    if (ey < 0.0f) ey = settings::getscreeny() + ey;
    //ey *= settings::getuiscale();
    return ey;
}

float uicontroller::getscaled(float s)
{
    return settings::getuiscale() * s;
}

float uicontroller::scalex(float xpos)
{
    return xpos * (float)settings::getscreenx();
}

float uicontroller::scaley(float ypos)
{
    return ypos * (float)settings::getscreeny();
}

float uicontroller::getcenterx(float width)
{
    return ((float)settings::getscreenx() - width) / 2.0f;
}

float uicontroller::getwcenterx(float width, float windowwidth)
{
    return (windowwidth - width) / 2.0f;
}

float uicontroller::getcentery(float height)
{
    return ((float)settings::getscreeny() - height) / 2.0f;
}

void uicontroller::changefont(int newfont)
{
    if (activefont != newfont)
    {
        if (activefont > 0)
        {
            popfont();//pops the old font away
        }
        activefont = newfont;
        ImGui::PushFont(font[newfont-1]);
    }
}

void uicontroller::popfont()
{
    ImGui::PopFont();
    activefont = 0;
}

void uicontroller::setupimgui()
{
    // Setup Dear ImGui context
    const char* glsl_version = "#version 130";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    (io)->ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(windowmanager::sdlwindow, windowmanager::sdlglcontext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //(io->Fonts)->AddFontDefault();

    float uiscale = settings::getuiscale();

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, uiscale * 0.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, uiscale * 0.2f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, uiscale * 0.5f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, uiscale * 0.05f);



    updateuiscale();
}

void uicontroller::newuiscale()
{
    updatescale = true;
}

void uicontroller::updateuiscale()
{
    if (updatescale)
    {
        float uiscale = settings::getuiscale();

        (io->Fonts)->Clear();

        font[0] = (io->Fonts)->AddFontFromFileTTF("data/rob.ttf", uiscale);//, NULL, (io->Fonts)->GetGlyphRangesJapanese());
        font[1] = (io->Fonts)->AddFontFromFileTTF("data/rob.ttf", uiscale * 2.1f);

        (io->Fonts)->Build();

        ImGui_ImplOpenGL3_CreateFontsTexture();

        //ImGui_ImplOpenGL3_DestroyDeviceObjects();

        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();

        /*ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, uiscale * 0.6f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, uiscale * 0.2f);*/

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, uiscale * 0.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, uiscale * 0.2f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, uiscale * 0.5f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, uiscale * 0.05f);

        uielement::updateuiscale();

        updatescale = false;
    }
}

void uicontroller::destroyimgui()
{
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
