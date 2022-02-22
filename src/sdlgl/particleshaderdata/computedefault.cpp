#include "consts.h"
#include "computedefault.h"

#include "systemmanager.h"
#include "uielement.h"
#include "shadercontroller.h"

computedefault::computedefault(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
    particleshadername = "Default";
}

computedefault::~computedefault()
{
    //dtor
}

void computedefault::renderpanel()
{
    uielement::text("Gravity:");
    uielement::sliderint(1, 2000, gravity, 1);

    uielement::spacing();

    uielement::text("Simulation mode:");

    std::string curshader = "Simple";
    if (shadermode == 1) curshader = "Interact";
    if (shadermode == 2) curshader = "Merge";

    if (uielement::begindropdown("##computemode", curshader))
    {
        if (uielement::addlistelement("Simple", (shadermode == 0) ? true : false)) shadermode = 0;
        if (uielement::addlistelement("Interact", (shadermode == 1) ? true : false)) shadermode = 1;
        if (uielement::addlistelement("Merge", (shadermode == 2) ? true : false)) shadermode = 2;

        uielement::enddropdown();
    }

    if (shadermode == 1)
    {
        uielement::text("Velocity transfer:");
        uielement::sliderint(0, 100, veltransfer, 1);

        uielement::text("Mass transfer:");
        uielement::sliderint(0, 100, masstransfer, 1);
    }

    if (shadermode != 0)
    {
        if (shadermode == 1) uielement::text("Interaction distance:");
        if (shadermode == 2) uielement::text("Merge distance:");
        uielement::sliderint(1, 2000, interactiondistance, 1);
    }
}

void computedefault::senduniforms()
{

    glUniform1i(shadercontroller::getuniformid("particlenumber"), systemmanager::get_currentparticlenumber());
    glUniform1f(shadercontroller::getuniformid("gravity"), (float)gravity/100000000.0f);

    if (shadermode == 1)
    {
        glUniform1f(shadercontroller::getuniformid("veltransfer"), (float)veltransfer/100.0f);
        glUniform1f(shadercontroller::getuniformid("masstransfer"), (float)masstransfer/100.0f);
    }

    if (shadermode != 0) glUniform1f(shadercontroller::getuniformid("interactiondistance"), (float)interactiondistance/1000.0f);
}

int computedefault::getvalue(int valueid)
{
    switch (valueid)
    {
    case 0:
        return gravity;
    case 1:
        return veltransfer;
    case 2:
        return masstransfer;
    case 3:
        return interactiondistance;
    case 4:
        return shadermode;
    }

    return 0;
}

void computedefault::setvalue(int valueid, int value)
{
    switch (valueid)
    {
    case 0:
        gravity = value; break;
    case 1:
        veltransfer = value; break;
    case 2:
        masstransfer = value; break;
    case 3:
        interactiondistance = value; break;
    case 4:
        shadermode = value; break;
    }
}
