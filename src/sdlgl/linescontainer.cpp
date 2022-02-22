#include "consts.h"
#include "linescontainer.h"

#include "cameracontroller.h"
#include "shadercontroller.h"
#include "utils.h"

linescontainer::linescontainer()
{
    opacity = 1.0f;
}

linescontainer::~linescontainer()
{
    cleanup();
}

void linescontainer::cleanup()
{
    glDeleteBuffers(1, &pointsvbo);
    glDeleteVertexArrays(1, &pointsvao);
}

void linescontainer::initialize()
{
    glGenBuffers(1, &pointsvbo);
    glGenVertexArrays(1, &pointsvao);
}

void linescontainer::addline(glm::vec3 start, glm::vec3 startcolor, glm::vec3 stop, glm::vec3 stopcolor)
{
    lines.push_back(glm::vec4(start.x, start.y, start.z, utils::packrgb(startcolor)));
    lines.push_back(glm::vec4(stop.x, stop.y, stop.z, utils::packrgb(stopcolor)));
}

void linescontainer::addline(glm::vec3 start, glm::vec3 stop, glm::vec3 color)
{
    lines.push_back(glm::vec4(start.x, start.y, start.z, utils::packrgb(color)));
    lines.push_back(glm::vec4(stop.x, stop.y, stop.z, utils::packrgb(color)));
}

void linescontainer::addline(glm::vec4 start, glm::vec4 stop)
{
    lines.push_back(start);
    lines.push_back(stop);
}

void linescontainer::clearlines()
{
    lines.clear();
}

void linescontainer::setvbos()
{
    glBindVertexArray(pointsvao);

    glBindBuffer(GL_ARRAY_BUFFER, pointsvbo);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(GLfloat) * 4, &lines[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void linescontainer::render()
{
    shadercontroller::activateshader(shaderids::line);
    glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));
    glUniform1f(shadercontroller::getuniformid("opacity"), opacity);

    glBindVertexArray(pointsvao);

    glDrawArrays(GL_LINES, 0, lines.size());

    glBindVertexArray(0);
}
