#include "consts.h"
#include "singleparticlegenerator.h"
#include "uielement.h"

singleparticlegenerator::singleparticlegenerator()
{
    //ctor
    title = "Single particle generator";

    updategeneration();
}

singleparticlegenerator::~singleparticlegenerator()
{
    //dtor
}

void singleparticlegenerator::boxcontents()
{
    bool changed = false;

    uielement::text("Particle mass: ");
    changed |= uielement::sliderint(1, 99000, smass, 1);

    uielement::text("Coarse position x,y,z: ");
    static glm::ivec3 coarseposition = glm::ivec3(0);
    changed |= uielement::sliderint(-99000, 99000, coarseposition.x, 1);
    changed |= uielement::sliderint(-99000, 99000, coarseposition.y, 1);
    changed |= uielement::sliderint(-99000, 99000, coarseposition.z, 1);

    uielement::text("Fine position x,y,z: ");
    static glm::ivec3 fineposition = glm::ivec3(0);;
    changed |= uielement::sliderint(-999, 999, fineposition.x, 1);
    changed |= uielement::sliderint(-999, 999, fineposition.y, 1);
    changed |= uielement::sliderint(-999, 999, fineposition.z, 1);

    sposition = coarseposition + fineposition;

    uielement::text("Velocity x,y,z: ");
    changed |= uielement::sliderint(-200, 200, svelocity.x, 1);
    changed |= uielement::sliderint(-200, 200, svelocity.y, 1);
    changed |= uielement::sliderint(-200, 200, svelocity.z, 1);

    if (changed)
        updategeneration();
}

void singleparticlegenerator::generatenewparticles()
{
    cleardata();

    gen_posmasses.push_back(glm::vec4((float)sposition.x / c_positionfactor, (float)sposition.y / c_positionfactor, (float)sposition.z / c_positionfactor,
                                       (float)smass / c_massfactor));
    gen_velocities.push_back(glm::vec4((float)svelocity.x / c_velocityfactor, (float)svelocity.y / c_velocityfactor, (float)svelocity.z / c_velocityfactor, 0.0f));
}
