#include "consts.h"
#include "sceneloadsystem.h"
#include "systemsinfomanager.h"
#include "systemmanager.h"

sceneloadsystem::sceneloadsystem()
{
    //ctor
}

sceneloadsystem::~sceneloadsystem()
{
    //dtor
}

void sceneloadsystem::initialize()
{

}

void sceneloadsystem::show()
{
    systemsinfomanager::load_systems_info();
    windowmanager::setvsync(true);
}

void sceneloadsystem::hide()
{

}

void sceneloadsystem::render()
{
    uielement::beginwindow("loadsys##w", glm::vec2(uielement::getcenterx(30.0f), 0.0f), glm::vec2(30.0f, 0.0f));

    uicontroller::changefont(uicontroller::FONT_LARGE);
    uielement::centertext("Megaplanetos", 1.5f, 30.0f);

    uicontroller::changefont(uicontroller::FONT_NORMAL);
    uielement::centertext("Select system to load", 5.0f, 30.0f);
    float centerx = uicontroller::getwcenterx(15.0f, 30.0f);


    if (uielement::button("Blank system", glm::vec2(centerx, (10.0f)), glm::vec2(15.0f, 1.5f)))
    {
        systemmanager::clear_system();
        scenec::changeactivescene(sceneids::game);
    }

    uielement::centertext("Load system:", 14.0f, 30.0f);

    const std::vector<systemsinfomanager::system_info>& systemsdata = systemsinfomanager::get_systems_info();

    uielement::beginchild("systems", glm::vec2(2.0f, 15.0f), glm::vec2(26.0f, -20.0f), true, false, true);

    if (systemsdata.size() == 0)
        uielement::text("No systems found in folder "+systemsavefolder);

    int index = 0;
    for (const systemsinfomanager::system_info& s : systemsdata)
    {
        if (uielement::button("load##"+std::to_string(index), glm::vec2(1.0f, 1.5f + index * 4.0f), glm::vec2(4.0f, 3.5f)))
        {
            //load system
            systemmanager::load_system(s.filename);
            scenec::changeactivescene(sceneids::game);
        }
        uielement::text(s.name + " ("+s.filename+")", glm::vec2(6.0f, 1.5f + index * 4.0f));
        uielement::text(s.description, glm::vec2(6.0f, 2.5f + index * 4.0f));
        uielement::text("Particles: "+std::to_string(s.particlenumber), glm::vec2(6.0f, 3.5f + index * 4.0f));

        index ++;
    }

    uielement::endchild();


    if (uielement::button("back", glm::vec2(centerx, -4.5f), glm::vec2(15.0f, 2.5f))) scenec::goback();

    uielement::endwindow();
}

void sceneloadsystem::update()
{

}

void sceneloadsystem::destroy()
{

}
