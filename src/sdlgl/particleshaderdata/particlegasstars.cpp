#include "consts.h"
#include "particlegasstars.h"

#include "uielement.h"

#include "cameracontroller.h"
#include "shadercontroller.h"

particlegasstars::particlegasstars(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
    particleshadername = "Gas and stars";
}

particlegasstars::~particlegasstars()
{
    //dtor
}

void particlegasstars::renderpanel()
{
    uielement::text("Star size:");
    uielement::sliderint(1, 500, starsize, 1);

    uielement::text("Gas->star mass (x100):");
    uielement::sliderint(1, 900, starmass, 1);

    uielement::text("Cloud size:");
    uielement::sliderint(1, 2000, cloudsize, 1);

    uielement::text("Cloud opacity:");
    uielement::sliderint(1, 100, opacity, 1);
}

void particlegasstars::senduniforms()
{
    glm::vec3 campos = cameracontroller::getcampos();
    glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));
    glUniform3f(shadercontroller::getuniformid("campos"), campos.x, campos.y, campos.z);
    glUniform1f(shadercontroller::getuniformid("cloudsize"), cloudsize);
    glUniform1f(shadercontroller::getuniformid("starsize"), starsize);
    glUniform1f(shadercontroller::getuniformid("starmass"), starmass);
    glUniform1f(shadercontroller::getuniformid("opacity"), (float)opacity/100.0f);
}
