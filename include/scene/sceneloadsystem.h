#ifndef SCENELOADSYSTEM_H
#define SCENELOADSYSTEM_H

#include "scene.h"
#include "scenec.h"
#include "uicontroller.h"

class sceneloadsystem : public scene
{
    public:
        sceneloadsystem();
        virtual ~sceneloadsystem();

        void render();
        void show();
        void hide();
        void destroy();

        void update();
        void initialize();
    protected:

    private:
};

#endif // SCENELOADSYSTEM_H
