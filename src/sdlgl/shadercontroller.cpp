#include "consts.h"
#include "shadercontroller.h"
#include "shaderdata.h"
#include "particlegasclouds.h"
#include "particlesimple.h"
#include "particlegasstars.h"
#include "particlecircles.h"
#include "particlestars.h"
#include "computedefault.h"
#include "computemove.h"
#include "uielement.h"

namespace shadercontroller
{
    void addshader(shaderids shaderid, std::string filename, std::vector<std::string> uniforms, shadertype stype);
    shader* getshader(shaderids shaderid);

    std::unordered_map<uint32_t, shader> shaders;
    shader* activeshader = nullptr;

    shaderids currentparticleshader {shaderids::particlegasstars};

    std::unordered_map<uint32_t, std::unique_ptr<shaderdata>> particleshaderdata;
    std::unordered_map<uint32_t, std::unique_ptr<shaderdata>> computeshaderdata;
}

std::unique_ptr<shaderdata>& shadercontroller::getcomputeshaderdata(shaderids shaderid)
{
    return computeshaderdata[static_cast<uint32_t>(shaderid)];
}

shader* shadercontroller::getshader(shaderids shaderid)
{
    return &(shaders.at(static_cast<uint32_t>(shaderid)));
}

void shadercontroller::addshader(shaderids shaderid, std::string filename, std::vector<std::string> uniforms, shadertype stype)
{
    //shaders.emplace(shaderid, (filename, uniforms));

    shaders.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderid)), std::forward_as_tuple(filename, uniforms, stype)); //constructes directe i mappet for å unngå destructor crap
}

void shadercontroller::activatecurrentparticleshader()
{
    shadercontroller::activateshader(currentparticleshader);

    particleshaderdata[static_cast<uint32_t>(currentparticleshader)]->senduniforms();
}

void shadercontroller::rendercomputeshaderpanels()
{
    computeshaderdata[static_cast<uint32_t>(shaderids::compute_gravity)]->renderpanel();
    computeshaderdata[static_cast<uint32_t>(shaderids::compute_moveparticles)]->renderpanel();
}

void shadercontroller::sendcomputeshaderuniforms(shaderids shaderid)
{
    computeshaderdata[static_cast<uint32_t>(shaderid)]->senduniforms();
}

void shadercontroller::renderparticleshaderlist()
{
    if (uielement::begindropdown("##shaderlist", particleshaderdata[static_cast<uint32_t>(currentparticleshader)]->particleshadername))
    {
        for (auto& shad : particleshaderdata)
        {
            if (uielement::addlistelement((shad.second)->particleshadername, (currentparticleshader == (shad.second)->thisshaderid)))
            {
                currentparticleshader = (shad.second)->thisshaderid;
            }
        }
        uielement::enddropdown();
    }

    particleshaderdata[static_cast<uint32_t>(currentparticleshader)]->renderpanel();
}

void shadercontroller::initialize()
{
    //common shaders
    addshader(shaderids::tempparticle, "tempparticle", {"pv"}, shadertype::fragvert);
    addshader(shaderids::line, "line", {"pv", "opacity"}, shadertype::fragvert);

    //particle shaders
    addshader(shaderids::particlegas, "particlegas", {"pv", "campos", "cloudsize", "opacity"}, shadertype::fragvert);
    particleshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::particlegas)), std::forward_as_tuple(std::make_unique<particlegasclouds>(shaderids::particlegas)));

    addshader(shaderids::particlegasstars, "particlegasstars", {"pv", "campos", "cloudsize", "opacity", "starsize", "starmass"}, shadertype::fragvert);
    particleshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::particlegasstars)), std::forward_as_tuple(std::make_unique<particlegasstars>(shaderids::particlegasstars)));

    addshader(shaderids::particlestars, "particlestars", {"pv", "campos", "opacity", "starsize"}, shadertype::fragvert);
    particleshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::particlestars)), std::forward_as_tuple(std::make_unique<particlestars>(shaderids::particlestars)));

    addshader(shaderids::particlesimple, "particlesimple", {"pv", "particlesize"}, shadertype::fragvert);
    particleshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::particlesimple)), std::forward_as_tuple(std::make_unique<particlesimple>(shaderids::particlesimple)));

    addshader(shaderids::particlecircles, "particlecircles", {"pv", "campos", "circlesize", "opacity", "massdependence", "masscolor"}, shadertype::fragvert);
    particleshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::particlecircles)), std::forward_as_tuple(std::make_unique<particlecircles>(shaderids::particlecircles)));

    //compute shaders
    addshader(shaderids::compute_gravity_interact, "compute_gravity_interact", {"particlenumber", "gravity", "veltransfer", "masstransfer", "interactiondistance"}, shadertype::compute);
    addshader(shaderids::compute_gravity_simple, "compute_gravity_simple", {"particlenumber", "gravity"}, shadertype::compute);
    addshader(shaderids::compute_gravity_merge, "compute_gravity_merge", {"particlenumber", "gravity", "interactiondistance"}, shadertype::compute);
    computeshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::compute_gravity)), std::forward_as_tuple(std::make_unique<computedefault>(shaderids::compute_gravity)));
    addshader(shaderids::compute_moveparticles_interact, "compute_moveparticles_interact", {"slowdown"}, shadertype::compute);
    addshader(shaderids::compute_moveparticles_simple, "compute_moveparticles_simple", {"slowdown"}, shadertype::compute);
    addshader(shaderids::compute_moveparticles_merge, "compute_moveparticles_merge", {"slowdown"}, shadertype::compute);
    computeshaderdata.emplace(std::piecewise_construct, std::forward_as_tuple(static_cast<uint32_t>(shaderids::compute_moveparticles)), std::forward_as_tuple(std::make_unique<computemove>(shaderids::compute_moveparticles)));
}

GLuint shadercontroller::getuniformid(std::string uniformname)
{
    return activeshader->getuniformid(uniformname);
}

void shadercontroller::activateshader(shaderids shaderid)
{
    activeshader = getshader(shaderid);
    activeshader->activate();
}
