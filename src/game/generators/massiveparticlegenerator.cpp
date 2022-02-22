#include "consts.h"
#include "massiveparticlegenerator.h"

#include "uielement.h"

massiveparticlegenerator::massiveparticlegenerator()
{
    //ctor
    title = "Massive particle generator";

    updategeneration();
}

massiveparticlegenerator::~massiveparticlegenerator()
{
    //dtor
}

void massiveparticlegenerator::boxcontents()
{
    bool changed = false;
    int xmass = std::round(c_massfactor);
    uielement::text("Particle mass (x "+std::to_string(xmass)+"): ");
    changed |= uielement::sliderint(1000, 990000, smass, 1);

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

void massiveparticlegenerator::generatenewparticles()
{
    cleardata();

    gen_posmasses.push_back(glm::vec4((float)sposition.x / c_positionfactor, (float)sposition.y / c_positionfactor, (float)sposition.z / c_positionfactor,
                                       (float)smass));
    gen_velocities.push_back(glm::vec4((float)svelocity.x / c_velocityfactor, (float)svelocity.y / c_velocityfactor, (float)svelocity.z / c_velocityfactor, 0.0f));
}
