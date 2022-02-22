#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include "windowmanager.h"

namespace timekeeper
{
    void timekeeping();
    void initialize();

    float get_deltatime();
    float get_timefactor();
    float get_cappeddeltatime();
    int get_fps();
};

#endif // TIMEKEEPER_H
