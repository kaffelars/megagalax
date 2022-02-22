#include "consts.h"
#include "computemove.h"

#include "uielement.h"
#include "shadercontroller.h"

computemove::computemove(shaderids shaderid) : shaderdata{shaderid}
{
    //ctor
}

computemove::~computemove()
{
    //dtor
}

void computemove::renderpanel()
{
    uielement::text("Slowdown:");
    uielement::sliderint(1, 500, slowdown, 1);
}

void computemove::senduniforms()
{
    glUniform1f(shadercontroller::getuniformid("slowdown"), slowdown);
}

int computemove::getvalue(int valueid)
{
    return slowdown;
}

void computemove::setvalue(int valueid, int value)
{
    slowdown = value;
}
