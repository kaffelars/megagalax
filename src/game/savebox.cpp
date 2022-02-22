#include "consts.h"
#include "savebox.h"

#include "uielement.h"
#include "systemmanager.h"
#include "inputmanager.h"
#include "boxcontroller.h"

namespace savebox
{
    bool showbox {false};

    std::string name;
    std::string description;
    std::string filename;
}

void savebox::show()
{
    showbox = true;
    name = systemmanager::get_current_name();
    description = systemmanager::get_current_desc();
    inputmanager::pauseallkeyfunctions();
}

void savebox::hide()
{
    showbox = false;
    inputmanager::resumeallkeyfunctions();
}

bool savebox::active()
{
    return showbox;
}

void savebox::render()
{
    uielement::beginwindow("Save system to disk", glm::vec2(0.0f), glm::vec2(20.0f, 20.0f), true, true, false, true);
    uielement::text("Warning! Files will get overwritten with no warning.");
    uielement::spacing();

    uielement::text("System name: ");
    uielement::inputtext("##Sname", &name);
    uielement::text("System description: ");
    uielement::inputtext("##Sdesc", &description);
    uielement::text("File name: ");
    uielement::inputtext(".txt", &filename);

    uielement::spacing();

    if (uielement::button("Save", glm::vec2(9.0f, 2.0f)))
    {
        if (filename != "")
        {
            if (systemmanager::save_system(name, description, filename))
                boxcontroller::addokaybox("Saved", filename+".txt\nsuccessfully saved!");
            else
                boxcontroller::addokaybox("Save error", filename+".txt\nnot saved due to some error...");

            hide();
        }

    };
    if (uielement::button("Cancel", glm::vec2(9.0f, 2.0f))) hide();
    uielement::endwindow();
}
