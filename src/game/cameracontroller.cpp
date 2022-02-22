#include "consts.h"
#include "cameracontroller.h"
#include "settings.h"
#include "windowmanager.h"
#include "linescontainer.h"
#include "shadercontroller.h"
#include "utils.h"

namespace cameracontroller
{
    glm::mat4 projection;
    glm::mat4 viewmatrix;

    glm::mat4 pvmatrix;

    glm::vec3 cameraoffset = glm::vec3(0.0f);
    glm::vec3 camerapos = glm::vec3(25.0, -25.0f, 25.0f);

    void updatecamerapos();

    glm::vec2 rotation = glm::vec2(0.0f, 1.0f);
    float zoom = 25.0f;

    bool freelookcamera {false};

    void updatepvmatrix();

    void updateviewmatrix();

    void updatecentervao();

    linescontainer centervao;

    bool rendercenter = false;
    bool showcenter = true;
}

void cameracontroller::resetcamera()
{
    rotation = glm::vec2(0.0f, 1.0f);
    if (freelookcamera)
        cameraoffset = glm::vec3(21.04f, -13.5f, 0.0f);
    else
        cameraoffset = glm::vec3(0.0f);
    //camerapos = glm::vec3(0.0f);
    zoom = 25.0f;

    updatecamerapos();
    updateviewmatrix();
}

void cameracontroller::changeshowcenter(bool onoff)
{
    showcenter = onoff;
}

bool& cameracontroller::getshowcenter()
{
    return showcenter;
}

void cameracontroller::initialize()
{
    freelookcamera = settings::getisetting(setting_enum::cameramode);

    resetcamera();

    updatecamerapos();
    updateviewmatrix();
    updateprojection();

    centervao.initialize();

    updatecentervao();
}

void cameracontroller::updatecentervao()
{
    centervao.clearlines();

    float rgbx = utils::packrgb(glm::vec3(1.0f, 0.0f, 0.0f));
    float rgby = utils::packrgb(glm::vec3(0.0f, 1.0f, 0.0f));
    float rgbz = utils::packrgb(glm::vec3(0.0f, 0.0f, 1.0f));

    float csize = 1.0f;

    centervao.addline(glm::vec4(0.0f+cameraoffset.x, 0.0f+cameraoffset.y, 0.0f+cameraoffset.z, rgbx),
                       glm::vec4(csize+cameraoffset.x, 0.0f+cameraoffset.y, 0.0f+cameraoffset.z, rgbx));
    centervao.addline(glm::vec4(0.0f+cameraoffset.x, 0.0f+cameraoffset.y, 0.0f+cameraoffset.z, rgby),
                       glm::vec4(0.0f+cameraoffset.x, -csize+cameraoffset.y, 0.0f+cameraoffset.z, rgby));
    centervao.addline(glm::vec4(0.0f+cameraoffset.x, 0.0f+cameraoffset.y, 0.0f+cameraoffset.z, rgbz),
                       glm::vec4(0.0f+cameraoffset.x, 0.0f+cameraoffset.y, csize+cameraoffset.z, rgbz));

    centervao.setvbos();
}

void cameracontroller::rendercentervao()
{
    if (showcenter)
        rendercenter = true;
}

glm::vec3 cameracontroller::getcampos()
{
    if (freelookcamera)
    {
        return cameraoffset;
    }
    else
    {
        return camerapos+cameraoffset;
    }
}

void cameracontroller::render()
{
    if (rendercenter && !freelookcamera)
    {
        centervao.render();
        rendercenter = false;
    }
}

glm::mat4& cameracontroller::getpvmatrix()
{
    updatepvmatrix();
    return pvmatrix;
}

void cameracontroller::movecamera(glm::vec3 movement)
{
    float movespeed = settings::getmovespeed();

    if (freelookcamera)
    {
        glm::vec3 camdir = glm::vec3(camerapos.x, camerapos.y, camerapos.z);

        camdir = glm::normalize(camdir);

        glm::vec3 hmovement = glm::vec3(0.0f);

        if (movement.z != 0.0f)
        {
            hmovement.x += movement.z * camdir.x * movespeed;
            hmovement.y += movement.z * camdir.y * movespeed;
            hmovement.z += movement.z * camdir.z * movespeed;
        }
        if (movement.x != 0.0f)
        {
            hmovement.x -= movement.x * camdir.z * movespeed;
            hmovement.z += movement.x * camdir.x * movespeed;
        }

        cameraoffset += hmovement;
    }
    else
    {
        glm::vec2 dir = glm::vec2(-camerapos.x, -camerapos.z);

        dir = glm::normalize(dir);

        glm::vec3 hmovement = glm::vec3(0.0f);

        hmovement.x = dir.x * movement.z;
        hmovement.z = dir.y * movement.z;

        hmovement.x += -dir.y * movement.x;
        hmovement.z += dir.x * movement.x;

        cameraoffset += hmovement * settings::getmovespeed();
    }

    cameraoffset.y += movement.y * settings::getmovespeed();

    updateviewmatrix();
    updatecentervao();

    rendercentervao();
}

void cameracontroller::rotatecamera(glm::vec2 deltarotation)
{
    if (freelookcamera)
    {
        rotation.x -= deltarotation.x;
        rotation.y -= deltarotation.y * (settings::getisetting(setting_enum::invert_y) == 0 ? 1.0f : -1.0f);
    }
    else
    {
        rotation += deltarotation;
    }

    if (rotation.y < 0.0002f) rotation.y = 0.0001f;
    if (rotation.y > 3.1415f) rotation.y = 3.1315;

    if (rotation.x < 0.0f) rotation.x += 6.28318f;
    if (rotation.x > 6.283185f) rotation.x -= 0.0001f;

    updatecamerapos();
}

void cameracontroller::changezoom(float change)
{
    if (freelookcamera)
    {
        zoom = 25.0f;
    }
    else
    {
        zoom += change;

        if (zoom < 2.0f) zoom = 2.0f;

        updatecamerapos();
    }
}

void cameracontroller::updatepvmatrix()
{
    pvmatrix = projection * viewmatrix;
}

void cameracontroller::updatecamerapos()
{
    camerapos.x = (cos(rotation.x) * sin(rotation.y)) * (zoom);
    camerapos.z = (sin(rotation.x) * sin(rotation.y)) * (zoom);
    camerapos.y = -cos(rotation.y) * zoom;

    camerapos *=  (freelookcamera == true ? -1.0f : 1.0f);

    updateviewmatrix();
}

void cameracontroller::updateviewmatrix()
{
    if (freelookcamera)
    {
        viewmatrix = glm::lookAt
        (
            cameraoffset,
            camerapos+cameraoffset,
            glm::vec3(0,-1,0)
        );
    }
    else
    {
        viewmatrix = glm::lookAt
        (
            camerapos+cameraoffset,
            cameraoffset,
            glm::vec3(0,-1,0)
        );
    }
}

void cameracontroller::setfreelook(bool freelook)
{
    if (freelook != freelookcamera) togglecameramode();
}

void cameracontroller::togglecameramode()
{
    freelookcamera = !freelookcamera;

    settings::setsetting(setting_enum::cameramode, freelookcamera);

    resetcamera();

    updatecamerapos();
    updatecentervao();
}

void cameracontroller::updateprojection()
{
    projection = glm::perspective
    (
        settings::getfovradians(),
        windowmanager::windowaspectratio(),
        0.1f,
        1000.0f
    );
}
