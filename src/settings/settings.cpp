#include "consts.h"
#include "settings.h"

namespace settings
{
    namespace //private
    {
        std::vector<setting> setts;// map?
    }
}

void settings::loadsettings()
{
    if (!settings::setts.empty()) settings::setts.clear();//renser settings

    //settings to look for
    setts.emplace_back(setting("screenx", 0, 800));//video
    setts.emplace_back(setting("screeny", 0, 600));
    setts.emplace_back(setting("fov", 0, 85));
    setts.emplace_back(setting("vsync", 0, 1));
    setts.emplace_back(setting("uiscale", 1, 20.0f));
    setts.emplace_back(setting("mousesens", 1, 300.0f));
    setts.emplace_back(setting("simulateinbg", 0, 1));
    setts.emplace_back(setting("cameramode", 0, 1));
    setts.emplace_back(setting("invert_y", 0, 1));
    setts.emplace_back(setting("movespeed", 0, 100));
    setts.emplace_back(setting("shaderworkgroups", 0, 512));
    setts.emplace_back(setting("simulateeveryn", 0, 1));


    pugi::xml_document doc;

    std::string fname = datasavefolder+"settings.txt";
    pugi::xml_parse_result result = doc.load_file(fname.c_str());

    pugi::xml_node settingsdata = doc.child("settings");

    for (setting& s: setts)
    {
        if (pugi::xml_node specificsetting = settingsdata.child(s.identifier.c_str()))
        {
            if (s.type == 0)
                s.value = specificsetting.attribute("value").as_int();
            if (s.type == 1)
                s.value = specificsetting.attribute("value").as_float();
        }
    }
}


void settings::savesettings()
{
    if (!setts.empty())
    {
        pugi::xml_document doc;
        pugi::xml_node settingsdata = doc.append_child("settings");

        for (setting& s: setts)
        {
            pugi::xml_node specificsetting = settingsdata.append_child(s.identifier.c_str());
            specificsetting.append_attribute("type") = s.type;
            if (s.type == 0)
                specificsetting.append_attribute("value") = std::get<int>(s.value);
            else if (s.type == 1)
                specificsetting.append_attribute("value") = std::get<float>(s.value);
        }

        std::string file = datasavefolder+"settings.txt";
        doc.save_file(file.c_str());
    }
}

void settings::setsetting(setting_enum s, bool value) //not necessary
{
    setsetting(s, (int)value);
}

void settings::setsetting(setting_enum s, int value)
{
    if (setts[static_cast<uint32_t>(s)].type == 0)
    {
        setts[static_cast<uint32_t>(s)].value = value;
        savesettings();
    }
}

void settings::setsetting(setting_enum s, float value)
{
    if (setts[static_cast<uint32_t>(s)].type == 1)
    {
        setts[static_cast<uint32_t>(s)].value = value;
        savesettings();
    }
}


int settings::getisetting(setting_enum s)
{
    if (setts[static_cast<uint32_t>(s)].type == 0)
        return std::get<int>(setts[static_cast<uint32_t>(s)].value);
    else
        return 0;
}

float settings::getfsetting(setting_enum s)
{
    if (setts[static_cast<uint32_t>(s)].type == 1)
        return std::get<float>(setts[static_cast<uint32_t>(s)].value);
    else
        return 0;
}


//functions for lazy people
int settings::getscreenx()
{
    return settings::getisetting(setting_enum::screen_x);
}

int settings::getscreeny()
{
    return settings::getisetting(setting_enum::screen_y);
}

float settings::getuiscale()
{
    float uiscale = settings::getfsetting(setting_enum::uiscale);
    if (uiscale >= 1.0f && uiscale <= 50.0f)
        return uiscale;
    else
        return 20.0f;
}

float settings::getmovespeed()
{
    float mspeed = settings::getisetting(setting_enum::movespeed);
    return mspeed / 100.0f;
}

float settings::getfovradians()
{
    return glm::radians((float)settings::getisetting(setting_enum::fov));
}
