#ifndef SCENEGAME_H
#define SCENEGAME_H

#include "scene.h"

#include "objectsgenerator.h"

class scenegame : public scene
{
    public:
        scenegame();
        virtual ~scenegame();

        void render();
        void show();
        void hide();
        void destroy();

        void update();
        void initialize();

        std::vector<objectsgenerator*> objgenerators;

        objectsgenerator* currentobjgenerator;

        float movementvelocity;

    protected:

    private:
        bool showinfopanel {true};
        void movecamera();
        void togglepause();
        void renderstoragepointspanel();

        bool showdeletebox {false};

        void renderinfopanel();
        void rendersidepanel();

        void showobjgenerator(int id);

        void renderdeletebox();
};

#endif // SCENEGAME_H
