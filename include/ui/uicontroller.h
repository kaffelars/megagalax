#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include "windowmanager.h"
#include "uielement.h"

namespace uicontroller
{

    extern int activefont;
    //static ImFont* bigfont;//2
    //static const char* glsl_version;
    void setupimgui();
    void destroyimgui();

    void changefont(int newfont);

    void updateuiscale();
    void newuiscale();

    void popfont();


    bool uimouse();
    void render();

    float xpos(float ex);
    float ypos(float ey);
    float scalex(float x);
    float scaley(float y);
    float getcenterx(float width);
    float getwcenterx(float width, float windowwidth);
    float getcentery(float height);

    float getscaled(float s);

    enum fonttyp
    {
        FONT_NONE,
        FONT_NORMAL,
        FONT_LARGE
    };
}


#endif // UICONTROLLER_H
