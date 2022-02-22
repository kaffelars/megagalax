#include "consts.h"
#include "currentgeneratorpoints.h"
#include "cameracontroller.h"
#include "shadercontroller.h"
#include "utils.h"
#include "linescontainer.h"

namespace currentgeneratorpoints
{
    GLuint pointsvbo, pointsvao;

    int numparticles = 0;

    bool showvelocities {true};

    linescontainer lines;

    std::vector<glm::vec4> velocitylines;
}

void currentgeneratorpoints::toggleshowvelocities(bool turnon)
{
    showvelocities = turnon;
}

bool currentgeneratorpoints::isshowingvelocities()
{
    return showvelocities;
}

void currentgeneratorpoints::initialize()
{
    glGenBuffers(1, &pointsvbo);
    glGenVertexArrays(1, &pointsvao);
    glBindVertexArray(pointsvao);
    glBindBuffer(GL_ARRAY_BUFFER, pointsvbo);
    glBufferData(GL_ARRAY_BUFFER, c_maxparticles * 16, NULL, GL_STATIC_DRAW); //16 = vec4
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(0);

    lines.initialize();
}

void currentgeneratorpoints::updatevao(std::vector<glm::vec4>& points, std::vector<glm::vec4>& velocities)
{
    glBindVertexArray(pointsvao);

    glBindBuffer(GL_ARRAY_BUFFER, pointsvbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * 16, &points[0]);//, GL_STATIC_DRAW);

    numparticles = points.size();

    //if (showvelocities)
    //{
        velocitylines.clear();

        lines.clearlines();

        for (int a = 0; a < points.size(); a++)
        {
            float linecolorstart = utils::packrgb(glm::vec3(0.1f, 0.3f, 0.1f));
            float linecolorend = utils::packrgb(glm::vec3(0.3f, 0.7f, 0.3f));
            lines.addline(glm::vec4(points[a].x, points[a].y, points[a].z, linecolorstart),
                          glm::vec4(points[a].x + velocities[a].x, points[a].y + velocities[a].y, points[a].z + velocities[a].z, linecolorend));
        }

        lines.setvbos();
    //}
}


void currentgeneratorpoints::render()
{
    if (numparticles > 0)
    {
        shadercontroller::activateshader(shaderids::tempparticle);
        glUniformMatrix4fv(shadercontroller::getuniformid("pv"), 1, GL_FALSE, &(cameracontroller::getpvmatrix()[0][0]));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(pointsvao);

        glDrawArrays(GL_POINTS, 0, numparticles);

        if (showvelocities)
        {
            lines.render();
        }
    }
}

void currentgeneratorpoints::cleanup()
{
    glDeleteBuffers(1, &pointsvbo);
    glDeleteVertexArrays(1, &pointsvao);
}
