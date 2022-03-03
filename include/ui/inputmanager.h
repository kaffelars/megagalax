#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "windowmanager.h"
#include "uicontroller.h"

namespace inputmanager
{
    enum key_mods
    {
        mod_none = 0,
        mod_ctrl = 1,
        mod_shift = 2,
        mod_crab = 4
    };

    extern int recordpress;
    extern void recordnextpress();
    extern bool keyrecorded;

    void initialize();
    void logcursorpos();

    void hidemouse();
    void showmouse();

    glm::ivec2 getmousedelta();
    bool ismousehidden();

    glm::ivec2 getcursorpos();


    void clearallkeyfunctions();
    void pausekeyfunctions();
    void unpausekey(key_enum);
    void resumekeyfunctions();

    void resumeallkeyfunctions();
    void pauseallkeyfunctions();
    void pausekeyfunctionsexcept(std::vector<key_enum> keys);

    void setkeyfunction(key_enum keyid, std::function<void()> kclick, key_event kevent);

    void processevent(SDL_Event& e);
    void processheldkeys();

    struct keydata
    {
        std::string keyname;
        std::string keydescription; //eks "forward", "cancel" osv.
        key_type ktype {key_type::keyboard};

        keydata(std::string name, std::string description, key_type ktyp) : keyname(name), keydescription(description), ktype(ktyp)
        {

        }

        bool held {false};
        bool clicked {false};
        int value {0};//for eks scroll

        std::function<void()> keyfunction[3];//funker ikke for mus/scroll

        void setkeyclick(std::function<void()> kclick, key_event kevent)
        {
            keyfunction[static_cast<uint32_t>(kevent)] = kclick;
        }

        void clearkeyclicks()
        {
            for (int a = 0; a < 3; a++)
                keyfunction[a] = nullptr;
        }
    };

    const std::vector<keydata>& getkeymap();
};

#endif // INPUTMANAGER_H
