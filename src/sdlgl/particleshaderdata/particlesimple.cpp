#include "consts.h"
#include "particlesimple.h"

#include "uielement.h"

#include "cameracontroller.h"
#include "shadercontroller.h"

particlesimple::particlesimple(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
    particleshadername = "Simple point shader";
}

particlesimple::~particlesimple()
{
    //dtor
}

void particlesimple::renderpanel()
{
    int particlesize_temp = particlesize;

    uielement::text("Particle size:");
    uielement::sliderint(1, 50, particlesize_temp, 1);

    if (particlesize_temp != particlesize)
        particlesize = particlesize_temp;
}

void particlesimple::senduniforms()
{
    glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));
    glUniform1f(shadercontroller::getuniformid("particlesize"), particlesize);
}
