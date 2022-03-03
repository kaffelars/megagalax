#include "consts.h"
#include "scenesettings.h"
#include "inputmanager.h"
#include "scenec.h"
#include "cameracontroller.h"

scenesettings::scenesettings()
{
    //ctor
}

scenesettings::~scenesettings()
{
    //dtor
}

void scenesettings::initialize()
{
    waitingforkeypress = -1;

    curselect = 0;
}

void scenesettings::show()
{
    inputmanager::clearallkeyfunctions();
    inputmanager::setkeyfunction(key_enum::escape, [&](){scenec::goback();}, key_event::clicked);
    windowmanager::setvsync(true);
}

void scenesettings::hide()
{

}

void scenesettings::render()
{
    std::string selections[2] = {"video", "show keys"};


    //settings
    uielement::beginwindow("peppe", glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), false, false);

    uicontroller::changefont(uicontroller::FONT_LARGE);
    uielement::centertext(selections[curselect].c_str(), 0.5f, 0.0f);

    uicontroller::changefont(uicontroller::FONT_NORMAL);

    int tempselect = curselect;

    for (int a = 0; a < 2; a++)
    {
        if (a == curselect)
        {
            //uicontroller::text(selections[a].c_str(), glm::vec2(1.5f, 5.5f + (a*10.0f)));
            uielement::button(selections[curselect].c_str(), glm::vec2(1.25f, 6.5f + (a*3.75f)), glm::vec2(15.0f, 2.5f), true);
        }
        else
        {
            if (uielement::button(selections[a].c_str(), glm::vec2(1.25f, 6.5f + (a*3.75f)), glm::vec2(15.0f, 2.5f))) tempselect = a;
        }
    }

    curselect = tempselect;

    if (uielement::button("back", glm::vec2(1.25f, -3.75f), glm::vec2(15.0f, 2.5f))) scenec::goback();

    uielement::setposition(17.5, 6.5f);

    uielement::beginchild("heks", glm::vec2(0), glm::vec2(uielement::getscaledscreenx() - 18.5f, uielement::getscaledscreeny()-7.75f), true, true);

    if (curselect == 0) video();
    if (curselect == 1) keybinds();

    ImGui::EndChild();

    uielement::endwindow();
}


void scenesettings::video()
{
    uielement::text("Ui scale (1 = smallest, 50 = largest): ");
    int uiscaletemp = settings::getuiscale();
    if (uielement::sliderint(1, 50, uiscaletemp, 1))
    {
        settings::setsetting(setting_enum::uiscale, (float)uiscaletemp);
        uicontroller::newuiscale();
    }

    uielement::spacing();

    bool vsync = settings::getisetting(setting_enum::vsync);
    if (uielement::checkbox("Vsync (in-game)", vsync))
    {
        settings::setsetting(setting_enum::vsync, vsync);
        windowmanager::setvsync(vsync);
    }

    uielement::spacing();

    uielement::text("Camera field of view (vertical): ");
    int fov = settings::getisetting(setting_enum::fov);
    if (uielement::sliderint(30, 180, fov, 1))
    {
        settings::setsetting(setting_enum::fov, fov);
        cameracontroller::updateprojection();
    }

    uielement::spacing();

    float mousesens = settings::getfsetting(setting_enum::mousesens);
    uielement::text("Camera movement sensitivity (10 = highest, 1000 = lowest): ");
    int mtemp = mousesens;
    if (uielement::sliderint(10, 1000, mtemp, 1))
    {
        settings::setsetting(setting_enum::mousesens, (float)mtemp);
        uicontroller::newuiscale();
    }

    uielement::spacing();

    int movespeed = settings::getisetting(setting_enum::movespeed);
    uielement::text("Camera movement speed");
    if (uielement::sliderint(1, 1000, movespeed, 1))
    {
        settings::setsetting(setting_enum::movespeed, movespeed);
    }

    uielement::spacing();

    bool simbg = settings::getisetting(setting_enum::simulateinbg);
    if (uielement::checkbox("Simulate while window doesn't have focus", simbg))
    {
        settings::setsetting(setting_enum::simulateinbg, simbg);
    }

    bool cammode = settings::getisetting(setting_enum::cameramode);
    if (uielement::checkbox("Free look camera", cammode))
    {
        settings::setsetting(setting_enum::cameramode, cammode);
        cameracontroller::setfreelook(cammode);
    }

    bool invert_y = settings::getisetting(setting_enum::invert_y);
    uielement::checkbox("Invert y-axis when in free look camera", invert_y);
    if (invert_y!=settings::getisetting(setting_enum::invert_y))
    {
        settings::setsetting(setting_enum::invert_y, invert_y);
    }

    uielement::spacing();
    uielement::text("Simulate every n rendered frames: ");
    int simeveryn = settings::getisetting(setting_enum::simulateeveryn);
    if (uielement::sliderint(1, 100, simeveryn, 1))
    {
        settings::setsetting(setting_enum::simulateeveryn, simeveryn);
    }
}

void scenesettings::keybinds()
{
    const std::vector<inputmanager::keydata>& keydata = inputmanager::getkeymap();

    uielement::begintable("keys table", 2);

    for (const inputmanager::keydata& k : keydata)
    {
        uielement::nexttablerow();
        uielement::nexttablecol();
        uielement::text(k.keydescription);
        uielement::nexttablecol();
        uielement::text(k.keyname);

    }

    uielement::endtable();
}

void scenesettings::update()
{

}

void scenesettings::destroy()
{

}
