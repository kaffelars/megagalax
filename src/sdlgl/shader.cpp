#include "consts.h"
#include "shader.h"
#include "messagelogger.h"

shader::shader(std::string filename, std::vector<std::string> uniformnames, shadertype styp)
{
    stype = styp;
    setupshader(filename);
    for (std::string u:uniformnames)
    {
        GLuint uniformloc = glGetUniformLocation(shaderprogram, u.c_str());
        uniforms.emplace(u, uniformloc);
    }
}

shader::~shader()
{
    //dtor
    cleanup();
}

void shader::cleanup()
{
    glUseProgram(0);

    for (auto i : shaderids)
        glDetachShader(shaderprogram, i);

    glDeleteProgram(shaderprogram);

    for (auto i : shaderids)
        glDeleteShader(i);
}

void shader::setupshader(std::string filename)
{
    shaderprogram = glCreateProgram();

    if (stype == shadertype::fragvert)
    {
        std::string vertex = "data/shaders/" + filename + "vert.glsl";
        std::string fragment = "data/shaders/" + filename + "frag.glsl";

        loadshaderprogram(vertex, GL_VERTEX_SHADER);
        loadshaderprogram(fragment, GL_FRAGMENT_SHADER);
    }
    else if (stype == shadertype::compute)
    {
        std::string compute = "data/shaders/" + filename + ".glsl";
        loadshaderprogram(compute, GL_COMPUTE_SHADER);
    }

    glLinkProgram(shaderprogram);

    int islinked;
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int*)&islinked);

    if (!islinked)
        printlinkingerror(shaderprogram);

    activate();
}

void shader::activate()
{
    glUseProgram(shaderprogram);
}

GLuint shader::getuniformid(std::string uniformname)
{
    return uniforms.at(uniformname);
}

bool shader::loadshaderprogram(std::string shado, GLenum shadertype)
{
    messagelogger::logmessage("shader", "Loading shader: " + shado, warningseverity::information);

    std::ifstream filu(shado.c_str());
    std::stringstream buffer;
    buffer << filu.rdbuf();

    std::string shaderinnhold = buffer.str();

    char* src = const_cast<char*>(shaderinnhold.c_str());
    int32_t strsize = shaderinnhold.length();

    int shaderid = glCreateShader(shadertype);
    glShaderSource(shaderid, 1, &src, &strsize);
    glCompileShader(shaderid);

    int wascompiled = 0;
    glGetShaderiv(shaderid, GL_COMPILE_STATUS, &wascompiled);

    printcompilationlog(shaderid);

    if (wascompiled)
    {
        glAttachShader(shaderprogram, shaderid);
        shaderids.push_back(shaderid);
        return true;
    }

    return false;
}

void shader::printlinkingerror(int32_t shaderid)
{
    int leng;
    glGetProgramiv(shaderid, GL_INFO_LOG_LENGTH, &leng);
    char* errorlog = new char[leng];
    glGetProgramInfoLog(shaderprogram, leng, &leng, errorlog);

    std::string info (errorlog);
    messagelogger::logmessage("shader", "Linking error: " + info, warningseverity::severe);

    delete errorlog;
    return;
}

void shader::printcompilationlog(int32_t shaderid)
{
    int leng;
    glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &leng);

    if (leng)
    {
        char* infolog = new char[leng];
        glGetShaderInfoLog(shaderid, leng, &leng, infolog);
        std::string info (infolog);
        messagelogger::logmessage("shader", "Compile info: " + info, warningseverity::severe);

        delete infolog;
    }
}
