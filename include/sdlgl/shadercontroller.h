#ifndef SHADERCONTROLLER_H
#define SHADERCONTROLLER_H

#include "shader.h"
#include "shaderdata.h"

namespace shadercontroller
{
    void initialize();
    void activateshader(shaderids shadernum);
    GLuint getuniformid(std::string uniformname);
    std::unique_ptr<shaderdata>& getcomputeshaderdata(shaderids shaderid);
    void activatecurrentparticleshader();
    void renderparticleshaderlist();
    void rendercomputeshaderpanels();
    void sendcomputeshaderuniforms(shaderids shaderid);
}


#endif // SHADERCONTROLLER_H
