#ifndef SETTINGS_H
#define SETTINGS_H

#include "pugixml.hpp"

namespace settings
{
    struct setting
    {
        std::string identifier;
        int type;
        std::variant<int, float> value;

        setting(std::string id, int typ, int defaultvalue) : identifier(id), type(typ), value(defaultvalue) {};
        setting(std::string id, int typ, float defaultvalue) : identifier(id), type(typ), value(defaultvalue) {};
    };

    void loadsettings();
    void savesettings();

    void setsetting(setting_enum s, bool value);
    void setsetting(setting_enum s, int value);
    void setsetting(setting_enum s, float value);

    int getisetting(setting_enum s);
    float getfsetting(setting_enum s);

    int getscreenx();
    int getscreeny();
    float getuiscale();
    float getmovespeed();
    float getfovradians();
}

#endif // SETTINGS_H
