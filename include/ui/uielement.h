#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "settings.h"
#include "uicontroller.h"

namespace uielement
{
    void resetcounter();

    void updateuiscale();

    bool sliderint(int from, int to, int& into, int division = 0);

    void setposition(float x, float y);

    std::string counterstr();

    bool button(std::string text, glm::vec2 pos, glm::vec2 buttonsize, bool disabled = false);
    bool button(std::string text, glm::vec2 buttonsize, bool disabled = false);
    bool buttonheld(std::string text, glm::vec2 pos, glm::vec2 buttonsize);

    void beginwindow(std::string windowtitle, glm::vec2 windowpos, glm::vec2 windowsize, bool titlebar = false, bool moveable = false, bool scrollbar = false, bool background = true);
    void endwindow();

    void beginchild(std::string windowtitle, glm::vec2 windowpos, glm::vec2 windowsize, bool scrollbar, bool background, bool position = false);
    void endchild();

    void spacing();

    float getscaledscreenx();
    float getscaledscreeny();
    float ypos(float y);

    //input text
    void inputtext(std::string label, std::string* text);

    //center
    float getcenterx(float width);
    float getwcenterx(float width, float windowwidth);

    //list
    bool addlistelement(std::string text, bool selected);

    //dropdown combo
    bool begindropdown(std::string id, std::string name);
    void enddropdown();

    //table
    void begintable(std::string name, int cols, bool borders = true);
    void nexttablecol();
    void nexttablerow();
    void endtable();

    //text
    void text(std::string text, glm::vec2 pos);
    void text(std::string text);
    void centertext(std::string text, float y, float winwidth = 0.0f);

    void setcursorpos(int cx, int cy);

    void addrect(glm::vec2 start, glm::vec2 rectsize, glm::vec3 col);

    void checkbox(bool &checked, glm::vec2 position);
    void checkbox(std::string text, bool &checked);
}

#endif // UIELEMENT_H
