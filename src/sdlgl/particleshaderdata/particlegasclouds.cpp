#include "consts.h"
#include "particlegasclouds.h"

#include "uielement.h"

#include "cameracontroller.h"
#include "shadercontroller.h"

particlegasclouds::particlegasclouds(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
    particleshadername = "Gas clouds";
}

particlegasclouds::~particlegasclouds()
{
    //dtor
}

void particlegasclouds::renderpanel()
{
    uielement::text("Cloud size:");
    uielement::sliderint(1, 2000, cloudsize, 1);

    uielement::text("Cloud opacity:");
    uielement::sliderint(1, 100, opacity, 1);
}

void particlegasclouds::senduniforms()
{
    glm::vec3 campos = cameracontroller::getcampos();
    glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));
    glUniform3f(shadercontroller::getuniformid("campos"), campos.x, campos.y, campos.z);
    glUniform1f(shadercontroller::getuniformid("cloudsize"), cloudsize);
    glUniform1f(shadercontroller::getuniformid("opacity"), (float)opacity/100.0f);
}
