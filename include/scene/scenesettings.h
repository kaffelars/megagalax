#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

#include "scene.h"

#include "inputmanager.h"
#include "shadercontroller.h"
#include "scenec.h"

#include "uicontroller.h"

class scenesettings : public scene
{
    public:
        scenesettings();
        virtual ~scenesettings();

        void render();
        void show();
        void hide();
        void update();
        void initialize();
        void destroy();

        void keybinds();
        void video();

        int curselect;

        bool goback;

        int waitingforkeypress;


    protected:

    private:
};

#endif // SCENESETTINGS_H
