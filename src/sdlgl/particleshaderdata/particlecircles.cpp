#include "consts.h"
#include "particlecircles.h"

#include "uielement.h"

#include "cameracontroller.h"
#include "shadercontroller.h"


particlecircles::particlecircles(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
    particleshadername = "Circles";
}

particlecircles::~particlecircles()
{
    //dtor
}

void particlecircles::renderpanel()
{
    uielement::text("Circle size:");
    uielement::sliderint(1, 1000, circlesize, 1);

    uielement::text("Circle opacity:");
    uielement::sliderint(1, 100, opacity, 1);

    uielement::text("Size mass dependence:");
    uielement::sliderint(0, 100, massdependence, 1);

    uielement::text("Color mass dependence:");
    uielement::sliderint(0, 100, masscolor, 1);
}

void particlecircles::senduniforms()
{
    glm::vec3 campos = cameracontroller::getcampos();
    glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));
    glUniform3f(shadercontroller::getuniformid("campos"), campos.x, campos.y, campos.z);
    glUniform1f(shadercontroller::getuniformid("circlesize"), circlesize);
    glUniform1f(shadercontroller::getuniformid("opacity"), (float)opacity/100.0f);
    glUniform1f(shadercontroller::getuniformid("massdependence"), (float)massdependence/100.0f);
    glUniform1f(shadercontroller::getuniformid("masscolor"), (float)masscolor/100.0f);
}
