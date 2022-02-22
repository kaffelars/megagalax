#include "consts.h"
#include "objectsgenerator.h"
#include "settings.h"
#include "uicontroller.h"
#include "shadercontroller.h"
#include "cameracontroller.h"
#include "systemmanager.h"
#include "currentgeneratorpoints.h"

objectsgenerator::objectsgenerator()
{
    //ctor
    position.y = -1.0f;
    title = "default";
}

objectsgenerator::~objectsgenerator()
{
    //dtor
}

void objectsgenerator::renderbox()
{
    startbox();
    boxcontents();
    endbox();
}

bool objectsgenerator::readytoapply()
{
    if (gen_posmasses.size() > 0 && gen_velocities.size() > 0)
        return true;

    return false;
}

int objectsgenerator::gettotalmass()
{
    int totmass = 0;
    for (glm::vec4& g : gen_posmasses)
    {
        totmass += g.a;
    }
    return totmass;
}

void objectsgenerator::startbox()
{
    uielement::beginwindow(title + "##boxbox", glm::vec2(0.0f), glm::vec2(14.8f, 30.0f), true, true, false, true);

    uielement::beginchild("applybox", glm::vec2(0.0f, 0.0f), glm::vec2(14.0f, 5.0f), false, false);

    if (uielement::button("Apply", glm::vec2(1.0f, 0.4f), glm::vec2(4.5f, 1.5f)))
    {
        if (readytoapply())
        {
            hidebox = true;

            systemmanager::addparticles(title, gen_posmasses, gen_velocities);
        }
    }
    if (uielement::button("Cancel", glm::vec2(8.5f, 0.4f), glm::vec2(4.5f, 1.5f))) hidebox = true;

    uielement::text("Total mass: "+std::to_string(gettotalmass())+"k");
    uielement::text("Actual particle num: "+std::to_string(gen_posmasses.size()));

    uielement::endchild();

    uielement::beginchild("settings", glm::vec2(0.0f, 2.5f), glm::vec2(14.0f, 22.5f), true, false);
}

void objectsgenerator::endbox()
{
    uielement::endchild();
    uielement::endwindow();
}

void objectsgenerator::updategeneration()
{
    generatenewparticles();
    updatevao();
}

void objectsgenerator::updatevao()
{
    currentgeneratorpoints::updatevao(gen_posmasses, gen_velocities);
}

void objectsgenerator::rendertempparticles()
{
    currentgeneratorpoints::render();
}

void objectsgenerator::cleardata()
{
    gen_posmasses.clear();
    gen_velocities.clear();
    //obj_masses.clear();
}
