#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include "scene.h"
#include "scenemainmenu.h"
#include "scenesettings.h"
#include "scenegame.h"
#include "sceneloadsystem.h"

namespace scenec
{
    void quitgame();
    void initialize();
    void changeactivescene(sceneids changeto);
    scene& getactivescene();
    void goback();

    extern bool quit;
};

#endif // SCENECONTROLLER_H
