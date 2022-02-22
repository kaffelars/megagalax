#include "consts.h"
#include "boxcontroller.h"

#include "uielement.h"

namespace boxcontroller
{
    std::vector<box> activeboxes;
    void renderokaybox(box& b);
    void removeboxes();
    void showboxes();
    uint32_t counter = 0;
}

void boxcontroller::addokaybox(std::string title, std::string text)
{
    box b;
    b.title = title;
    b.text = text;
    b.type = boxtype::okaybox;
    b.id = counter;

    counter++;

    activeboxes.push_back(b);
}

void boxcontroller::renderokaybox(box& b)
{
    uielement::beginwindow(b.title+std::string("##w")+std::to_string(b.id), glm::vec2(0.0f), glm::vec2(15.0f, 8.0f), true, true, false, true);
    uielement::centertext(b.text, 2.3f, 15.0f);
    if (uielement::button(std::string("Ok##b")+std::to_string(b.id), glm::vec2(4.5f, 5.0f), glm::vec2(6.0f, 2.0f))) b.removethis = true;
    uielement::endwindow();
}

void boxcontroller::updateboxes()
{
    showboxes();
    removeboxes();
}

void boxcontroller::showboxes()
{
    if (activeboxes.empty()) return;

    for (box& b : activeboxes)
    {
        if (b.type == boxtype::okaybox)
        {
            renderokaybox(b);
        }
    }
}

void boxcontroller::removeboxes()
{
    //remove okay'd boxes
    auto it = activeboxes.begin();
    while (it != activeboxes.end())
    {
        if (it->removethis)
        {
            it = activeboxes.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
