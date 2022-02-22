#include "consts.h"
#include "inputmanager.h"


namespace inputmanager
{
    glm::ivec2 cursorpos;
    glm::ivec2 deltamove;
    glm::ivec2 mousetrappos;
    //glm::ivec2 inputmanager::cursorposold;
    bool mousehidden = false;

    uint8_t currentmodifiers = 0;

    bool keyfunctionsactive = false;

    int recordpress = 0;
    //b_key recordedpress;

    uint8_t getmodifiers();

    std::string numbertokeyboardkeyname(int id);
    std::string numbertomousebuttonname(int id);

    //ny
    void addkey(key_type ktype, key_enum keyenum, int32_t keypress, key_mods kmod, std::string description);
    uint32_t getkeyid(key_type ktype, int32_t keypress, key_mods kmod);
    void processkey(key_type ktype, int32_t keypress, key_mods kmod, key_status kstatus);
    uint32_t getkeyidfromenum(key_enum keyenum);
    std::vector<uint32_t> heldkeys;
    std::vector<keydata> keymap;
    std::unordered_map<uint32_t, uint32_t> keyidtokeymap;
    std::unordered_map<uint32_t, uint32_t> keyenumtokeymap;

    std::vector<bool> keypaused;
}

const std::vector<inputmanager::keydata>& inputmanager::getkeymap()
{
    return keymap;
}

uint32_t inputmanager::getkeyid(key_type ktype, int32_t keypress, key_mods kmod)
{
    union keyk
    {
        struct ss
        {
            uint8_t scancode; //hvorfor ikke bare ha ktype og keypress og kmods
            uint8_t button;
            uint8_t scroll;
            uint8_t kmods;
        } s;

        uint32_t i;
    };

    keyk k;
    k.s.scancode = 0;
    k.s.button = 0;
    k.s.scroll = 0;
    k.s.kmods = kmod;

    switch (ktype)
    {
    case key_type::keyboard:
        k.s.scancode = keypress;
        break;
    case key_type::mousebutton:
        k.s.button = keypress;
        break;
    case key_type::mousescroll:
        if (keypress == 1)
            k.s.scroll = 1;
        else if (keypress == -1)
            k.s.scroll = 2;
        break;
    }

    return k.i;
}

void inputmanager::processheldkeys()
{
    for (uint32_t k_id : heldkeys)
    {
        keydata& key = keymap[k_id];
        if (!keypaused[k_id] && key.keyfunction[static_cast<uint32_t>(key_event::held)])
            key.keyfunction[static_cast<uint32_t>(key_event::held)]();
    }
}

void inputmanager::processkey(key_type ktype, int32_t keypress, key_mods kmod, key_status kstatus)
{
    uint32_t kid = getkeyid(ktype, keypress, kmod);

    if (keyidtokeymap.find(kid) == keyidtokeymap.end()) return;

    uint32_t k_id = keyidtokeymap.at(kid);
    keydata& key = keymap[k_id];

    if (kstatus == key_status::down)
    {
        key.clicked = false;
        if (key.held == false)
        {
            key.clicked = true;
            if (!keypaused[k_id] && key.keyfunction[static_cast<uint32_t>(key_event::clicked)])
                key.keyfunction[static_cast<uint32_t>(key_event::clicked)]();
        }
        heldkeys.push_back(k_id);
        key.held = true;

    }

    if (kstatus == key_status::up)
    {
        heldkeys.erase(std::remove(heldkeys.begin(), heldkeys.end(), k_id), heldkeys.end());
        key.held = false;
        key.clicked = false;
        if (!keypaused[k_id] && key.keyfunction[static_cast<uint32_t>(key_event::released)])
            key.keyfunction[static_cast<uint32_t>(key_event::released)]();
    }
}

void inputmanager::processevent(SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        {
            SDL_Scancode s_key = e.key.keysym.scancode;
            processkey(key_type::keyboard, s_key, mod_none, key_status::down);
            break;
        }
    case SDL_KEYUP:
        {
            SDL_Scancode s_key = e.key.keysym.scancode;
            processkey(key_type::keyboard, s_key, mod_none, key_status::up);
            break;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            int button = e.button.button;
            processkey(key_type::mousebutton, button, mod_none, key_status::down);
            break;
        }
    case SDL_MOUSEBUTTONUP:
        {
            int button = e.button.button;
            processkey(key_type::mousebutton, button, mod_none, key_status::up);
            break;
        }
    case SDL_MOUSEWHEEL:
        {
            if (e.wheel.y > 0)
                processkey(key_type::mousescroll, 1, mod_none, key_status::down);
            if (e.wheel.y < 0)
                processkey(key_type::mousescroll, -1, mod_none, key_status::down);
            break;
        }
    default:
        break;
    }

    processkey(key_type::mousescroll, 1, mod_none, key_status::up);
    processkey(key_type::mousescroll, -1, mod_none, key_status::up);
}

std::string inputmanager::numbertokeyboardkeyname(int id)
{
    std::string s = SDL_GetScancodeName(static_cast<SDL_Scancode>(id));//shady
    return s;
}

std::string inputmanager::numbertomousebuttonname(int id)
{
    if (id == 1) return "left";
    if (id == 2) return "middle";
    if (id == 3) return "right";

    return "button " + std::to_string(id);
}

void inputmanager::addkey(key_type ktype, key_enum keyenum, int32_t keypress, key_mods kmod, std::string description)
{
    std::stringstream keyname;

    switch (ktype)
    {
    case key_type::keyboard:
        keyname<<"key: ";
        keyname<<numbertokeyboardkeyname(keypress);
        break;
    case key_type::mousebutton:
        keyname<<"mouse button: ";
        keyname<<numbertomousebuttonname(keypress);
        break;
    case key_type::mousescroll:
        keyname<<"mouse scroll: "<<(keypress == 1 ? "up" : "down");break;
    }

    keymap.emplace_back(keydata(keyname.str(), description, ktype));
    uint32_t id = keymap.size()-1;

    uint32_t kid = getkeyid(ktype, keypress, kmod);
    uint32_t kenumid = static_cast<uint32_t>(keyenum);

    keyidtokeymap.insert(std::pair(kid, id));
    keyenumtokeymap.insert(std::pair(kenumid, id));
}

uint32_t inputmanager::getkeyidfromenum(key_enum keyenum)
{
    if (keyenumtokeymap.find(static_cast<uint32_t>(keyenum)) == keyenumtokeymap.end()) return 0;
    return keyenumtokeymap.at(static_cast<uint32_t>(keyenum));
}

void inputmanager::setkeyfunction(key_enum keyenum, std::function<void()> kclick, key_event kevent)
{
    uint32_t keyid = getkeyidfromenum(keyenum);
    keymap[keyid].setkeyclick(kclick, kevent);
}

void inputmanager::clearallkeyfunctions()
{
    for (keydata& k : keymap)
    {
        k.clearkeyclicks();
    }
}

void inputmanager::pausekeyfunctionsexcept(std::vector<key_enum> exceptionkeys)
{
    std::vector<uint32_t> exceptionkeysids;
    for (key_enum& k : exceptionkeys)
    {
        exceptionkeysids.push_back(keyenumtokeymap[static_cast<uint32_t>(k)]);
    }
    for (int a = 0; a < keymap.size(); a++)
    {
        if (std::find(exceptionkeysids.begin(), exceptionkeysids.end(), a) == exceptionkeysids.end())
        {
            keypaused[a] = true;
        }
        else
        {
            keypaused[a] = false;
        }
    }
}

void inputmanager::pauseallkeyfunctions()
{
    std::fill(keypaused.begin(), keypaused.end(), true);
}

void inputmanager::resumeallkeyfunctions()
{
    std::fill(keypaused.begin(), keypaused.end(), false);
}

void inputmanager::pausekeyfunctions()
{
    keyfunctionsactive = false;
}

void inputmanager::unpausekey(key_enum)
{

}

void inputmanager::resumekeyfunctions()
{
    keyfunctionsactive = true;
}

void inputmanager::initialize()
{
    addkey(key_type::keyboard, key_enum::escape, SDL_SCANCODE_ESCAPE, mod_none, "escape");

    addkey(key_type::mousescroll, key_enum::zoomin, 1, mod_none, "zoom in");
    addkey(key_type::mousescroll, key_enum::zoomout, -1, mod_none, "zoom out");

    addkey(key_type::mousebutton, key_enum::select, SDL_BUTTON_LEFT, mod_none, "select");
    addkey(key_type::mousebutton, key_enum::movecamera, SDL_BUTTON_RIGHT, mod_none, "move camera");

    addkey(key_type::keyboard, key_enum::forwardkey, SDL_SCANCODE_W, mod_none, "forward");
    addkey(key_type::keyboard, key_enum::backwardkey, SDL_SCANCODE_S, mod_none, "backward");
    addkey(key_type::keyboard, key_enum::left, SDL_SCANCODE_A, mod_none, "left");
    addkey(key_type::keyboard, key_enum::right, SDL_SCANCODE_D, mod_none, "right");
    addkey(key_type::keyboard, key_enum::up, SDL_SCANCODE_Q, mod_none, "up");
    addkey(key_type::keyboard, key_enum::down, SDL_SCANCODE_E, mod_none, "down");

    addkey(key_type::keyboard, key_enum::toggle_pause, SDL_SCANCODE_P, mod_none, "toggle pause");

    addkey(key_type::keyboard, key_enum::toggle_camera, SDL_SCANCODE_C, mod_none, "toggle camera");

    for (int a =0 ; a < keymap.size(); a++)
        keypaused.push_back(false);
}

void inputmanager::hidemouse()//hides and locks mouse
{
    if (!inputmanager::ismousehidden())
    {
        SDL_ShowCursor(SDL_DISABLE);
        inputmanager::mousehidden = true;
        inputmanager::mousetrappos = inputmanager::getcursorpos();
    }
}

void inputmanager::showmouse()
{
    if (inputmanager::ismousehidden())
    {
        SDL_ShowCursor(SDL_ENABLE);
        inputmanager::mousehidden = false;
    }
}

bool inputmanager::ismousehidden()
{
    return inputmanager::mousehidden;
}

void inputmanager::logcursorpos()
{
    glm::ivec2 cursorposold = inputmanager::cursorpos;
    SDL_GetMouseState(&inputmanager::cursorpos.x, &inputmanager::cursorpos.y);

    inputmanager::deltamove = inputmanager::cursorpos - cursorposold;

    if (inputmanager::ismousehidden()) //mus låst
    {
        //inputmanager::deltamove = inputmanager::cursorpos - inputmanager::mousetrappos;
        inputmanager::cursorpos = inputmanager::mousetrappos;
        SDL_WarpMouseInWindow(windowmanager::sdlwindow, inputmanager::mousetrappos.x, inputmanager::mousetrappos.y);
    }
}

glm::ivec2 inputmanager::getcursorpos()
{
    return inputmanager::cursorpos;
}

glm::ivec2 inputmanager::getmousedelta()
{
    return inputmanager::deltamove;
}


void inputmanager::recordnextpress()
{
    recordpress = 1;
}

uint8_t inputmanager::getmodifiers()
{
    /*uint8_t mods = 0;

    if (inputmanager::keys[SDL_SCANCODE_LCTRL].held || inputmanager::keys[SDL_SCANCODE_RCTRL].held)
    {
        mods = mods | mod_ctrl;
    }

    if (inputmanager::keys[SDL_SCANCODE_LSHIFT].held || inputmanager::keys[SDL_SCANCODE_RSHIFT].held)
    {
        mods = mods | mod_shift;
    }

    return mods;*/
    return 0;
}
