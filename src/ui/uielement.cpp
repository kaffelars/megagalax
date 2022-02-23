#include "consts.h"
#include "uielement.h"
#include "settings.h"

namespace uielement
{
    int activefont = 0;
    int counter = 0;
    bool windowbegun = false;
    glm::vec2 curwindowsize = glm::vec2(0.0f);

    float uiscale = 1.0f;
}

void uielement::updateuiscale()
{
    uiscale = settings::getuiscale();
}

void uielement::resetcounter()
{
    counter = 0;
}

void uielement::inputtext(std::string label, std::string* text)
{
    ImGui::InputText(label.c_str(), text);
}

bool uielement::sliderint(int from, int to, int& into, int division)
{
    std::string crab = "##slider"+counterstr();
    bool changed = ImGui::SliderInt(crab.c_str(), &into, from, to);
    if (division)
    {
        into /= division;
        into *= division;
    }

    return changed;
}

std::string uielement::counterstr()
{
    counter++;
    return std::to_string(counter-1);
}

void uielement::setposition(float x, float y)
{
    y = ypos(y);
    ImGui::SetCursorPosX((float)x*uiscale);
    ImGui::SetCursorPosY((float)y*uiscale);
}

float uielement::ypos(float y)
{
    if (y < 0.0f)
    {
        y = getscaledscreeny() + y;
    }
    return y;
}

float uielement::getscaledscreenx()
{
    return (float)settings::getscreenx() / uiscale;
}
float uielement::getscaledscreeny()
{
    return (float)settings::getscreeny() / uiscale;
}

void uielement::text(std::string text, glm::vec2 pos)
{
    setposition(pos.x, pos.y);

    ImGui::Text(text.c_str());
}


void uielement::centertext(std::string text, float y, float winwidth)
{
    if (winwidth == 0.0f) winwidth = settings::getscreenx() / uiscale;

    setposition(uielement::getwcenterx(ImGui::CalcTextSize(text.c_str(), nullptr).x/uiscale, winwidth), y);
    ImGui::Text(text.c_str());
}

void uielement::text(std::string text)
{
    ImGui::Text(text.c_str());
}

bool uielement::button(std::string text, glm::vec2 buttonsize, bool disabled) //negative pos = centered
{
    if (disabled)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }

    if (disabled)
    {
        ImGui::Button(text.c_str(), ImVec2(buttonsize.x*uiscale, buttonsize.y*uiscale));
        ImGui::PopStyleVar();
        return false;
    }
    else
    {
        return ImGui::Button(text.c_str(), ImVec2(buttonsize.x*uiscale, buttonsize.y*uiscale));
    }
}

void uielement::begintable(std::string name, int cols, bool borders)
{
    ImGui::BeginTable(name.c_str(), cols, borders == true ? ImGuiTableFlags_Borders: 0);
}

void uielement::nexttablecol()
{
    ImGui::TableNextColumn();
}

void uielement::nexttablerow()
{
    ImGui::TableNextRow();
}

void uielement::endtable()
{
    ImGui::EndTable();
}

float uielement::getcenterx(float width)
{
    return (((float)settings::getscreenx() - (width*uiscale)) / 2.0f) / uiscale;
}

float uielement::getwcenterx(float width, float windowwidth)
{
    return (windowwidth - width) / 2.0f;
}


bool uielement::button(std::string text, glm::vec2 pos, glm::vec2 buttonsize, bool disabled) //negative pos = centered
{
    setposition(pos.x, pos.y);

    return button(text, buttonsize, disabled);
}

bool uielement::checkbox(std::string text, bool &checked)
{
    return ImGui::Checkbox(text.c_str(), &checked);
}

bool uielement::checkbox(bool &checked, glm::vec2 position)
{
    setposition(position.x, position.y);

    std::string flepsi = "##c" + counterstr();

    return ImGui::Checkbox(flepsi.c_str(), &checked);
}

void uielement::addrect(glm::vec2 rectstart, glm::vec2 rectend, glm::vec3 col)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilled(ImVec2(rectstart.x*uiscale, rectstart.y*uiscale), ImVec2(rectend.x*uiscale, rectend.y*uiscale), IM_COL32(col.r, col.g, col.b, 255));
}

void uielement::setcursorpos(int cx, int cy)
{
    ImGui::SetCursorPosX((float)cx*uiscale);
    ImGui::SetCursorPosY((float)cy*uiscale);
}

void uielement::spacing()
{
    ImGui::Dummy(ImVec2(0.0f, settings::getuiscale()));
}

bool uielement::buttonheld(std::string text, glm::vec2 pos, glm::vec2 buttonsize)
{
    uielement::button(text, pos, buttonsize);

    return ImGui::IsItemActive();
}

bool uielement::begindropdown(std::string id, std::string name)
{
    return ImGui::BeginCombo(id.c_str(), name.c_str());
}

void uielement::enddropdown()
{
    ImGui::EndCombo();
}

bool uielement::addlistelement(std::string text, bool selected)
{
    return ImGui::Selectable(text.c_str(), selected);
}

void uielement::beginwindow(std::string windowtitle, glm::vec2 windowpos, glm::vec2 windowsize, bool titlebar, bool moveable, bool scrollbar, bool background)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    if (!titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!moveable) window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    if (!background) window_flags |= ImGuiWindowFlags_NoBackground;
    //window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    //style

    windowsize.y = ypos(windowsize.y);

    if (windowsize.x == 0.0f) windowsize.x = settings::getscreenx();
    else windowsize.x *= uiscale;

    if (windowsize.y == 0.0f) windowsize.y = settings::getscreeny();
    else windowsize.y *= uiscale;

    if (!moveable) ImGui::SetNextWindowPos(ImVec2(windowpos.x*uiscale, windowpos.y*uiscale));
    ImGui::Begin(windowtitle.c_str(), NULL, window_flags);
    ImGui::SetWindowSize(ImVec2(windowsize.x, windowsize.y));

    uicontroller::changefont(uicontroller::FONT_NORMAL);

    windowbegun = true;
    curwindowsize = windowsize;
}

void uielement::beginchild(std::string windowtitle, glm::vec2 windowpos, glm::vec2 windowsize, bool scrollbar, bool background, bool position)
{
    if (windowbegun)
    {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (!scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoNav;
        if (!background) window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

        //style
        windowsize.y = ypos(windowsize.y);

        if (position) setposition(windowpos.x, windowpos.y);
        ImGui::BeginChild(windowtitle.c_str(), ImVec2(windowsize.x*uiscale, windowsize.y*uiscale), window_flags);

        uicontroller::changefont(uicontroller::FONT_NORMAL);
    }
}

void uielement::endchild()
{
    if (uicontroller::activefont > 0)
    {
        uicontroller::popfont();
        uicontroller::activefont = 0;
    }

    ImGui::EndChild();
}

void uielement::endwindow()
{
    if (uicontroller::activefont > 0)
    {
        uicontroller::popfont();
        uicontroller::activefont = 0;
    }

    ImGui::End();
    windowbegun = false;
}
