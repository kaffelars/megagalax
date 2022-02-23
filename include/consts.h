#ifndef CONSTS_H_INCLUDED
#define CONSTS_H_INCLUDED


#define GL3_PROTOTYPES 1


#include "includes.h"

const int c_maxparticles = 98304; //asset(c_maxparticles % workgroups == 0)
const int program_version = 56; //update if it breaks loading old version systems
const std::string systemsavefolder = "./data/systems/";
const std::string datasavefolder = "./data/";

const float c_massfactor = 100.0f;
const float c_positionfactor = 1000.0f;
const float c_velocityfactor = 10.0f;

#include "enums.h"


#endif // CONSTS_H_INCLUDED
