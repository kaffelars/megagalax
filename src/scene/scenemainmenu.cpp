#include "consts.h"
#include "scenemainmenu.h"
#include "inputmanager.h"
#include "settings.h"

scenemainmenu::scenemainmenu()
{
    //ctor
}

scenemainmenu::~scenemainmenu()
{
    //dtor
}

void scenemainmenu::initialize()
{

}

void scenemainmenu::show()
{
    inputmanager::clearallkeyfunctions();
}

void scenemainmenu::hide()
{

}

void scenemainmenu::render()
{
    //main menu
    uielement::beginwindow("mainmenuwindow", glm::vec2(uielement::getcenterx(30.0f), 0.0f), glm::vec2(30.0f, 0.0f));

    uicontroller::changefont(uicontroller::FONT_LARGE);
    uielement::centertext("Megaplanetos", 1.5f, 30.0f);

    uicontroller::changefont(uicontroller::FONT_NORMAL);

    float centerx = uielement::getwcenterx(15.0f, 30.0f);

    if (uielement::button("Start", glm::vec2(centerx, 15.0f), glm::vec2(15.0f, 2.5f))) scenec::changeactivescene(sceneids::loadsystem);
    if (uielement::button("Settings", glm::vec2(centerx, 20.0f), glm::vec2(15.0f, 2.5f))) scenec::changeactivescene(sceneids::settings);
    uielement::button("Info", glm::vec2(centerx, 25.0f), glm::vec2(15.0f, 2.5f), true);
    if (uielement::button("Quit", glm::vec2(centerx, 37.5f), glm::vec2(15.0f, 2.5f))) scenec::quitgame();

    uielement::centertext("Program version: " + std::to_string(program_version), -3.0f, 30.0f);
    uielement::centertext("github.com/kaffelars/megagalax", -2.0f, 30.0f);

    uielement::endwindow();
}

void scenemainmenu::update()
{

}

void scenemainmenu::destroy()
{

}
