#include "scenegame.h"
#include "timekeeper.h"
#include "uicontroller.h"
#include "systemmanager.h"
#include "spheregenerator.h"
#include "singleparticlegenerator.h"
#include "massiveparticlegenerator.h"
#include "inputmanager.h"
#include "cameracontroller.h"
#include "simulationmanager.h"
#include "currentgeneratorpoints.h"
#include "gridcontroller.h"
#include "scenec.h"
#include "savebox.h"

scenegame::scenegame()
{
    //ctor
    currentobjgenerator = nullptr;
    movementvelocity = 0.1f;
}

scenegame::~scenegame()
{
    //dtor
}

void scenegame::initialize()
{
    objgenerators.emplace_back(new spheregenerator());
    objgenerators.emplace_back(new singleparticlegenerator());
    objgenerators.emplace_back(new massiveparticlegenerator());
}

void scenegame::showobjgenerator(int id)
{
    currentobjgenerator = objgenerators[id];
    currentobjgenerator->updatevao();
}

void scenegame::show()
{
    inputmanager::setkeyfunction(key_enum::forwardkey, [&](){cameracontroller::movecamera(glm::vec3(0.0f, 0.0f, movementvelocity));}, key_event::held);
    inputmanager::setkeyfunction(key_enum::backwardkey, [&](){cameracontroller::movecamera(glm::vec3(0.0f, 0.0f, -movementvelocity));}, key_event::held);
    inputmanager::setkeyfunction(key_enum::up, [&](){cameracontroller::movecamera(glm::vec3(0.0f, -movementvelocity, 0.0f));}, key_event::held);
    inputmanager::setkeyfunction(key_enum::down, [&](){cameracontroller::movecamera(glm::vec3(0.0f, movementvelocity, 0.0f));}, key_event::held);
    inputmanager::setkeyfunction(key_enum::left, [&](){cameracontroller::movecamera(glm::vec3(movementvelocity, 0.0f, 0.0f));}, key_event::held);
    inputmanager::setkeyfunction(key_enum::right, [&](){cameracontroller::movecamera(glm::vec3(-movementvelocity, 0.0f, 0.0f));}, key_event::held);
    inputmanager::setkeyfunction(key_enum::movecamera, [&](){movecamera();}, key_event::held);
    inputmanager::setkeyfunction(key_enum::zoomin, [&](){if (!uicontroller::uimouse()) cameracontroller::changezoom(-0.5f);}, key_event::clicked);
    inputmanager::setkeyfunction(key_enum::zoomout, [&](){if (!uicontroller::uimouse()) cameracontroller::changezoom(0.5f);}, key_event::clicked);
    inputmanager::setkeyfunction(key_enum::escape, [&](){scenec::changeactivescene(sceneids::settings);}, key_event::clicked);

    inputmanager::setkeyfunction(key_enum::toggle_pause, [&](){togglepause();}, key_event::clicked);

    inputmanager::setkeyfunction(key_enum::toggle_camera, [&](){cameracontroller::togglecameramode();}, key_event::clicked);
}

void scenegame::togglepause()
{
    if (!savebox::active())
    {
        simulationmanager::toggle_pause_simulation();
    }
}

void scenegame::movecamera()
{
    glm::vec2 md = inputmanager::getmousedelta();

    md /= settings::getfsetting(setting_enum::mousesens);

    cameracontroller::rotatecamera(md);

    cameracontroller::rendercentervao();
}

void scenegame::hide()
{
    inputmanager::clearallkeyfunctions();
}

void scenegame::render()
{
    glm::vec3 campos = cameracontroller::getcampos();

    if (simulationmanager::is_simulation_paused())
    {
        //simulation not running
        if (savebox::active())
        {
            savebox::render();
        }
        else
        {
            rendersidepanel();

            if (showdeletebox)
            {
                renderdeletebox();
            }
        }
    }
    else
    {
        //sim running
        systemmanager::download_current_obj_pos_values();
    }

    if (systemmanager::get_particlesortingactive())
    {
        systemmanager::sortparticles(campos);
    }



    //render particles
    shadercontroller::activatecurrentparticleshader();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    systemmanager::renderparticles();

    glDepthFunc(GL_LESS);
    gridcontroller::rendergrid();
    glDisable(GL_DEPTH_TEST);
    gridcontroller::rendergrid();

    if (simulationmanager::is_simulation_paused() && !savebox::active())
    {
        if (currentobjgenerator != nullptr)
        {
            currentobjgenerator->renderbox();

            currentobjgenerator->rendertempparticles();

            if (currentobjgenerator->hidebox)
            {
                currentobjgenerator->hidebox = false;
                currentobjgenerator = nullptr;
            }
        }
    }


    if (showinfopanel && !savebox::active())
    {
        renderinfopanel();
    }

    //camera
    cameracontroller::render();
}

void scenegame::renderdeletebox()
{
    uielement::beginwindow("Delete particles", glm::vec2(0.0f), glm::vec2(14.8f, 30.0f), true, true, false, true);
    if (uielement::button("Delete all particles", glm::vec2(9.0f, 2.0f))) systemmanager::clear_system();

    uielement::spacing();

    static int n = 5;
    uielement::text("Delete every n particles: ");
    uielement::sliderint(2, 100, n, 1);
    if (uielement::button("Delete##n", glm::vec2(9.0f, 2.0f))) systemmanager::deleteeverynparticles(n);

    uielement::spacing();

    uielement::text("Added particles:");
    int index = 0;

    uielement::begintable("deletos", 2, false);
    int grptodelete = -1;
    for (const systemmanager::particlegroup& p : systemmanager::getparticlegroups())
    {
        uielement::nexttablerow();
        uielement::nexttablecol();
        if (uielement::button("Delete##"+std::to_string(index), glm::vec2(4.0f, 2.0f))) grptodelete = index;

        uielement::nexttablecol();
        uielement::text(p.name);
        uielement::text(std::to_string(p.firstparticle) + " - " + std::to_string(p.finalparticle-1));

        index++;
    }
    if (grptodelete >= 0) systemmanager::deleteparticlegroup(grptodelete);
    uielement::endtable();

    uielement::spacing();
    if (uielement::button("Hide window", glm::vec2(9.0f, 2.0f))) showdeletebox = false;
    uielement::endwindow();
}

void scenegame::rendersidepanel()
{
    float top = (showinfopanel) ? 4.0f : 0.0f;
    uielement::beginwindow("sidepanel", glm::vec2(0.0f, top), glm::vec2(15.0f, -top), false, false, true, true);

    bool showvels = currentgeneratorpoints::isshowingvelocities();
    uielement::checkbox("Show temp. velocities", showvels);

    if (showvels != currentgeneratorpoints::isshowingvelocities())
    {
        currentgeneratorpoints::toggleshowvelocities(showvels);
    }

    uielement::checkbox("Render horiz. grid", gridcontroller::getgridrender(0));
    uielement::checkbox("Show camera center", cameracontroller::getshowcenter());
    uielement::checkbox("Show info box", showinfopanel);
    if (uielement::button("Reset camera", glm::vec2(9.0f, 2.0f))) cameracontroller::resetcamera();

    uielement::text("Add particles: ");
    for (int a = 0; a < objgenerators.size(); a++)
        if (uielement::button(objgenerators[a]->title, glm::vec2(12.0f, 2.0f))) showobjgenerator(a);

    uielement::text("Delete stuff:");
    if (uielement::button("Show delete box", glm::vec2(9.0f, 2.0f))) showdeletebox = true;//systemmanager::clear_system();

    uielement::spacing();

    renderstoragepointspanel();

    uielement::spacing();

    uielement::text("Particle shader: ");

    shadercontroller::renderparticleshaderlist();

    uielement::spacing();

    uielement::text("Simulation: ");

    shadercontroller::rendercomputeshaderpanels();

    bool sorting = systemmanager::get_particlesortingactive();
    uielement::checkbox("Sort particles", sorting);
    if (sorting != systemmanager::get_particlesortingactive()) systemmanager::setparticlesorting(sorting);

    uielement::spacing();

    if (uielement::button("Save system to disk", glm::vec2(12.0f, 2.0f))) savebox::show();
    if (uielement::button("Load system from disk", glm::vec2(12.0f, 2.0f))) scenec::changeactivescene(sceneids::loadsystem);

    uielement::endwindow();
}

void scenegame::renderstoragepointspanel()
{
    uielement::text("Temp. store/restore points:");

    if (systemmanager::systemstored(0)) if (uielement::button("Restore first unpause", glm::vec2(9.0f, 2.0f))) systemmanager::revertsystem(0);
    if (systemmanager::systemstored(1)) if (uielement::button("Restore previous unpause", glm::vec2(9.0f, 2.0f))) systemmanager::revertsystem(1);

    int storagepoints = systemmanager::getstoragepointnumber() - 2;

    if (storagepoints > 0)
    {
        uielement::begintable("storage", 3);
        int deletepoints = -1;
        std::string index;
        for (int a = 0; a < storagepoints; a++)
        {
            index = std::to_string(a);
            uielement::nexttablerow();
            uielement::nexttablecol();
            if (uielement::button("overwrite##"+index, glm::vec2(4.0f, 2.0f))) systemmanager::storesystem(a + 2);
            uielement::nexttablecol();
            if (uielement::button("restore##"+index, glm::vec2(4.0f, 2.0f))) systemmanager::revertsystem(a + 2);
            uielement::nexttablecol();
            if (uielement::button("clear##"+index, glm::vec2(4.0f, 2.0f))) deletepoints = a+2;
        }

        if (deletepoints > 0) systemmanager::deletestoragepoint(deletepoints);
        uielement::endtable();
    }

    if (uielement::button("Store system", glm::vec2(9.0f, 2.0f))) systemmanager::storesystem(-1);
}


void scenegame::renderinfopanel()
{
    uielement::beginwindow("infopanel", glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 4.0f));
    uielement::text(systemmanager::get_current_name());
    uielement::text("fps: "+std::to_string(timekeeper::get_fps()));
    uielement::text("particles: "+std::to_string(systemmanager::get_currentparticlenumber()));

    uielement::endwindow();
}

void scenegame::update()
{
    simulationmanager::simulate();

}

void scenegame::destroy()
{

}
