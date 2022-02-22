#include "consts.h"
#include "particlestars.h"

#include "uielement.h"

#include "cameracontroller.h"
#include "shadercontroller.h"

particlestars::particlestars(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
    particleshadername = "Only stars";
}

particlestars::~particlestars()
{
    //dtor
}

void particlestars::renderpanel()
{
    uielement::text("Star size:");
    uielement::sliderint(1, 500, starsize, 1);

    uielement::text("Star opacity:");
    uielement::sliderint(1, 100, opacity, 1);
}

void particlestars::senduniforms()
{
    glm::vec3 campos = cameracontroller::getcampos();
    glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));
    glUniform3f(shadercontroller::getuniformid("campos"), campos.x, campos.y, campos.z);
    glUniform1f(shadercontroller::getuniformid("starsize"), starsize);
    glUniform1f(shadercontroller::getuniformid("opacity"), (float)opacity/100.0f);
}
