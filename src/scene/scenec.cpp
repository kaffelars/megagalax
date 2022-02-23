#include "scenec.h"

namespace scenec
{
    void addscene(sceneids id, std::unique_ptr<scene> sceneptr);
    bool quit = false;

    sceneids activescene {sceneids::blank};
    sceneids previousscene {sceneids::blank};
    std::unordered_map<uint32_t, std::unique_ptr<scene>> scenes;
}

void scenec::quitgame()
{
    quit = true;
}

void scenec::initialize()
{
    addscene(sceneids::mainmenu, std::make_unique<scenemainmenu>());
    addscene(sceneids::settings, std::make_unique<scenesettings>());
    addscene(sceneids::game, std::make_unique<scenegame>());
    addscene(sceneids::loadsystem, std::make_unique<sceneloadsystem>());
}

void scenec::addscene(sceneids sceneid, std::unique_ptr<scene> sceneptr)
{
    uint32_t sceneid_i = static_cast<uint32_t>(sceneid);
    scenes.emplace(sceneid_i, std::move(sceneptr));
    scenes[sceneid_i] -> initialize();
}

void scenec::changeactivescene(sceneids changeto)
{
    if (changeto != activescene && changeto != sceneids::blank)
    {
        previousscene = activescene;
        if (activescene != sceneids::blank) getactivescene().hide();
        activescene = changeto;
        getactivescene().show();
    }
}

void scenec::goback()
{
    if (previousscene != sceneids::blank)
        changeactivescene(previousscene);
}

scene& scenec::getactivescene()
{
    return *(scenes[static_cast<int>(activescene)]);
}
