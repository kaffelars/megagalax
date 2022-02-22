#include "consts.h"
#include "gridcontroller.h"
#include "linescontainer.h"

namespace gridcontroller
{
    bool rendergrids[3] = {false, false, false};

    linescontainer lines[3];
}

void gridcontroller::initialize()
{
    for (int a = 0; a < 3; a++)
    {
        lines[a].initialize();
        lines[a].opacity = 0.3f;
    }


    int gridextent = 32;
    int gridgap = 2;
    glm::vec3 gridcolor = glm::vec3(0.0f, 0.7f, 0.3f);
    for (int a = -gridextent; a <= gridextent; a+=gridgap)
    {
        lines[0].addline(glm::vec3(a,0,-gridextent), glm::vec3(a,0,gridextent), gridcolor);
        lines[0].addline(glm::vec3(-gridextent,0,a), glm::vec3(gridextent,0,a), gridcolor);
    }

    lines[0].setvbos();
}

void gridcontroller::setgridrender(int gridid, bool renderthis)
{
    rendergrids[gridid] = renderthis;
}

bool& gridcontroller::getgridrender(int gridid)
{
    return rendergrids[gridid];
}

void gridcontroller::rendergrid()
{
    for (int a = 0; a < 3; a++)
    {
        if (rendergrids[a])
        {
            lines[a].render();
        }
    }
}
