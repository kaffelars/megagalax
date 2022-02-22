#include "consts.h"
#include "spheregenerator.h"
#include "uicontroller.h"
#include "settings.h"
#include "utils.h"

spheregenerator::spheregenerator()
{
    //ctor
    particlenumber = 5000;
    title = "Sphere generator";

    updategeneration();
}

spheregenerator::~spheregenerator()
{
    //dtor
}



void spheregenerator::boxcontents()
{
    bool changed = false;

    uielement::text("Number of particles (full sphere): ");
    changed |= uielement::sliderint(1, 25000, particlenumber, 1);

    uielement::text("Radius: ");
    changed |= uielement::sliderint(1, 50000, sradius, 1);

    uielement::text("Min radius: ");
    changed |= uielement::sliderint(0, 50000, sminradius, 1);

    if (sminradius > sradius) sminradius = sradius;

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

    uielement::text("Rotation x,y,z: ");
    changed |= uielement::sliderint(-90, 90, srotation.x, 1);
    changed |= uielement::sliderint(-90, 90, srotation.y, 1);
    changed |= uielement::sliderint(-90, 90, srotation.z, 1);

    uielement::text("Minimum and maximum mass: ");

    changed |= uielement::sliderint(1, 99000, sminmass, 1);
    changed |= uielement::sliderint(1, 99000, smaxmass, 1);

    if (sminmass > smaxmass) smaxmass = sminmass;
    if (smaxmass < sminmass) sminmass = smaxmass;


    uielement::text("Mass distribution: ");
    changed |= uielement::sliderint(-100, 100, massdistribution, 1);

    uielement::text("Mass radius distribution: ");
    changed |= uielement::sliderint(-100, 100, massradiusdistribution, 1);
    uielement::checkbox("Include y: ", includey);

    uielement::text("Point radius distribution: ");
    changed |= uielement::sliderint(-100, 100, pointradiusdistribution, 1);

    uielement::text("Truncate center: ");
    changed |= uielement::sliderint(0, 100, truncatecenter, 1);

    uielement::text("Rotation speed x,y,z: ");
    changed |= uielement::sliderint(-200, 200, vrotation.x, 1);
    changed |= uielement::sliderint(-200, 200, vrotation.y, 1);
    changed |= uielement::sliderint(-200, 200, vrotation.z, 1);


    uielement::text("Random velocity: ");
    changed |= uielement::sliderint(0, 200, randomvel, 1);

    uielement::text("Overall velocity x,y,z: ");
    changed |= uielement::sliderint(-200, 200, svelocity.x, 1);
    changed |= uielement::sliderint(-200, 200, svelocity.y, 1);
    changed |= uielement::sliderint(-200, 200, svelocity.z, 1);


    uielement::text("Flatten sphere x,y,z: ");
    changed |= uielement::sliderint(0, 100, flatten.x, 1);
    changed |= uielement::sliderint(0, 100, flatten.y, 1);
    changed |= uielement::sliderint(0, 100, flatten.z, 1);

    uielement::text("Truncate poles: ");
    changed |= uielement::sliderint(0, 100, truncate, 1);

    uielement::text("Cylinder: ");
    changed |= uielement::sliderint(0, 100, cylinder, 1);
    uielement::text("Cylinder x (0), y (1) or z (2): ");
    changed |= uielement::sliderint(0, 2, cylinderxyz, 1);

    if (changed)
        updategeneration();
}

void spheregenerator::generatenewparticles()
{
    cleardata();

    glm::vec3 aposition = sposition;
    aposition = aposition / c_positionfactor;

    glm::mat4 rotx = glm::rotate(glm::mat4(1.0f), glm::radians((float)srotation.x),glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 roty = glm::rotate(glm::mat4(1.0f), glm::radians((float)srotation.y),glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotz = glm::rotate(glm::mat4(1.0f), glm::radians((float)srotation.z),glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rotationmatrix = rotz * roty * rotx;

    glm::vec3 totvelocity = svelocity;
    totvelocity /= c_velocityfactor;


    float aradius = (float)sradius / c_positionfactor;

    float massdistributionfactor = ((float)massdistribution) / 100.0f; //-1 to 1
    float massradiusdistributionfactor = ((float)massradiusdistribution) / 100.0f; //-1 to 1

    float truncation = ((float)truncate / 100.0f);
    float randomvelfactor = (float)randomvel / 100.0f;
    glm::vec3 flattenfactor = flatten;
    flattenfactor /= 100.0f;

    glm::vec3 rotationfactor = vrotation; //-2 to 2
    rotationfactor /= 100.0f;

    float truncradius = ((float)truncatecenter / 100.0f) * aradius;

    float pointradiusdistributionfactor = (float)pointradiusdistribution / 100.0f;

    float cylinderfactor = (float)cylinder / 100.0f;

    std::default_random_engine generator;
    std::normal_distribution<float> point(0.0f, 1.0f);
    std::uniform_real_distribution<float> radius(((float)sminradius / (float)sradius), 1.0f);
    std::uniform_real_distribution<float> mass(0.0f, 1.0f);
    std::uniform_real_distribution<float> randomvel(-randomvelfactor, randomvelfactor);

    for (int a = 0; a < particlenumber; a++)
    {
        glm::vec4 pos = glm::vec4(0);
        glm::vec3 temppos = glm::vec3(0);


        //point position
        glm::vec3 normal;
        normal.x = point(generator);
        normal.y = point(generator);
        normal.z = point(generator);
        normal = glm::normalize(normal);

        //flattening
        normal *= (glm::vec3(1.0f) - flattenfactor);

        //distance from center
        float dist = std::cbrt(radius(generator));

        //create points, radius distribution
        float dfactor = 1.0f;
        if (pointradiusdistributionfactor > 0.0f)
            dfactor = std::pow(dist, 1.0f + glm::clamp(std::abs(pointradiusdistributionfactor), 0.0f, 1.0f) * 9.0f);//
        else
            dfactor = std::pow(dist, 1.0f - glm::clamp(std::abs(pointradiusdistributionfactor), 0.0f, 1.0f));//

        temppos = normal * dfactor * aradius;

        //truncate poles (y)
        if (temppos.y > (1.0f - truncation) * aradius || temppos.y < -(1.0f - truncation) * aradius) continue;

        //truncate center
        if (glm::length(glm::vec2(temppos.x, temppos.z)) < truncradius) continue;

        //cylindrize
        glm::ivec3 sxyz;
        switch(cylinderxyz)
        {
        case 0:
            sxyz = glm::vec3(1, 0, 2);break;
        case 1:
            sxyz = glm::vec3(0, 1, 2);break;
        default:
            sxyz = glm::vec3(0, 2, 1);break;
        }
        float yfactor = abs(temppos[sxyz[1]]) / aradius;
        float edgedist = aradius - glm::length(glm::vec2(temppos[sxyz[0]], temppos[sxyz[2]]));
        float centerdist = aradius - edgedist;
        if (centerdist > 0.0f)
        {
            temppos[sxyz[0]] += std::sin(std::asin(temppos[sxyz[0]] / centerdist)) * edgedist * cylinderfactor * yfactor;
            temppos[sxyz[2]] += std::sin(std::asin(temppos[sxyz[2]] / centerdist)) * edgedist * cylinderfactor * yfactor;
        }

        //setting pos
        pos.x = temppos.x;
        pos.y = temppos.y;
        pos.z = temppos.z;
        pos.a = 1.0f;

        //distofactor
        float lengo;
        if (includey)
            lengo = glm::length(glm::vec3(pos.x, pos.y, pos.z));
        else
            lengo = glm::length(glm::vec2(pos.x, pos.z));

        float distofactor = lengo / aradius;

        //rotation velocity
        glm::vec3 rotationvelocity[3] = {glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)};
        rotationvelocity[0].x = 0.0f;
        rotationvelocity[0].z = -pos.y;
        rotationvelocity[0].y = pos.z;

        rotationvelocity[1].x = -pos.z;
        rotationvelocity[1].z = pos.x;
        rotationvelocity[1].y = 0.0f;

        rotationvelocity[2].x = -pos.y;
        rotationvelocity[2].z = 0.0f;
        rotationvelocity[2].y = pos.x;


        //pos rotation
        pos = rotationmatrix * pos;

        //pos translation
        pos.x -= aposition.x;
        pos.y -= aposition.y;
        pos.z -= aposition.z;

        //particle mass
        float massfactor = std::pow(mass(generator), 1.0f + glm::clamp(std::abs(massdistributionfactor), 0.0f, 1.0f) * 29.0f);//

        if (massdistributionfactor > 0.0f)
            pos.a = (float)smaxmass * (1.0f - massfactor) + (float)sminmass * massfactor;
        else
            pos.a = (float)sminmass * (1.0f - massfactor) + (float)smaxmass * massfactor;


        float distmassfactor = 0.0f;

        if (massradiusdistributionfactor > 0.0f)
        {
            distmassfactor = std::abs(massradiusdistributionfactor) * distofactor;
        }
        else if (massradiusdistributionfactor < 0.0f)
        {
            distmassfactor = std::abs(massradiusdistributionfactor) * (1.0f - distofactor);
        }

        pos.a = pos.a * (1.0f - distmassfactor) + (float)sminmass * distmassfactor;

        pos.a /= c_massfactor;



        //position+mass finito
        gen_posmasses.push_back(pos);


        //velocity
        glm::vec4 vel;

        vel.x = randomvelfactor * (randomvel(generator));
        vel.y = randomvelfactor * (randomvel(generator));
        vel.z = randomvelfactor * (randomvel(generator));

        //adding rotation velocity
        for (int a = 0; a < 3; a++)
        {
            vel.x += rotationfactor[a] * rotationvelocity[a].x;
            vel.y += rotationfactor[a] * rotationvelocity[a].y;
            vel.z += rotationfactor[a] * rotationvelocity[a].z;
        }

        vel.a = 0.0f;

        //rotating velocity if sphere is rotated
        vel = rotationmatrix * vel;

        //adding overall sphere velocity
        vel.x += totvelocity.x;
        vel.y += totvelocity.y;
        vel.z += totvelocity.z;


        //velocity finito
        gen_velocities.push_back(vel);
    }
}
