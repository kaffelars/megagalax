#include "consts.h"
#include "simulationmanager.h"
#include "shadercontroller.h"

#include "systemmanager.h"

namespace simulationmanager
{
    bool simulation_paused = true;
    int workgroups = 512;//setting?
}

bool simulationmanager::is_simulation_paused()
{
    return simulation_paused;
}

void simulationmanager::initialize()
{


}

int simulationmanager::getcomputeshadervalue(shaderids shaderid, int valueid)
{
    return shadercontroller::getcomputeshaderdata(shaderid)->getvalue(valueid);
}

void simulationmanager::setcomputeshadervalue(shaderids shaderid, int valueid, int value)
{
    shadercontroller::getcomputeshaderdata(shaderid)->setvalue(valueid, value);
}

int simulationmanager::getcurrentgravity()
{
    return getcomputeshadervalue(shaderids::compute_gravity, 0);
}

int simulationmanager::getcurrentslowdown()
{
    return getcomputeshadervalue(shaderids::compute_moveparticles, 0);
}

void simulationmanager::setcurrentgravity(int grav)
{
    setcomputeshadervalue(shaderids::compute_gravity, 0, grav);
}

void simulationmanager::setcurrentslowdown(int sdown)
{
    setcomputeshadervalue(shaderids::compute_moveparticles, 0, sdown);
}

void simulationmanager::pause_simulation()
{
    if (!simulation_paused)
    {
        simulation_paused = true;
        systemmanager::download_current_obj_values();
    }
}

void simulationmanager::unpause_simulation()
{
    if (simulation_paused)
    {
        simulation_paused = false;
        systemmanager::unpause();
        systemmanager::upload_current_obj_values(); //noen grunn til å ikke ha denne i unpause?
    }

}

void simulationmanager::toggle_pause_simulation()
{
    if (simulation_paused)
        unpause_simulation();
    else
        pause_simulation();
}

void simulationmanager::simulate()
{
    if (!simulation_paused)
    {
        int simmode = shadercontroller::getcomputeshaderdata(shaderids::compute_gravity)->getvalue(4);

        //systemmanager::bindssbos();
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        //blabla do gpu stuff
        //calculate new velocities
        switch(simmode)
        {
        case 0:
            shadercontroller::activateshader(shaderids::compute_gravity_simple);break;
        case 1:
            shadercontroller::activateshader(shaderids::compute_gravity_interact);break;
        case 2:
            shadercontroller::activateshader(shaderids::compute_gravity_merge);break;
        }

        shadercontroller::sendcomputeshaderuniforms(shaderids::compute_gravity);
        glDispatchCompute(workgroups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        if (simmode == 2)
        {
            systemmanager::mergeparticles();
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        }


        //move particles
        switch(simmode)
        {
        case 0:
            shadercontroller::activateshader(shaderids::compute_moveparticles_simple);break;
        case 1:
            shadercontroller::activateshader(shaderids::compute_moveparticles_interact);break;
        case 2:
            shadercontroller::activateshader(shaderids::compute_moveparticles_merge);break;
        }
        shadercontroller::sendcomputeshaderuniforms(shaderids::compute_moveparticles);
        glDispatchCompute(workgroups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
}
