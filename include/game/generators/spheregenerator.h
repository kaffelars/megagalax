#ifndef SPHEREGENERATOR_H
#define SPHEREGENERATOR_H

#include "objectsgenerator.h"

class spheregenerator : public objectsgenerator
{
    public:
        spheregenerator();
        virtual ~spheregenerator();

        void boxcontents();
        void generatenewparticles();

    protected:

    private:
        int sradius {5000};
        int sminradius {0};
        bool includey {false};
        int truncatecenter {0};
        int cylinder {0};
        int cylinderxyz {1};
        glm::ivec3 sposition {glm::ivec3(0)};
        glm::ivec3 srotation {glm::ivec3(0)};
        glm::ivec3 svelocity {glm::ivec3(0)};
        int sminmass {25};
        int smaxmass {4500};
        int hollowness {0};
        int pointradiusdistribution{0};
        glm::ivec3 flatten {glm::ivec3(0)};
        int truncate {0};
        int randomvel {0};
        glm::ivec3 vrotation {glm::ivec3(0)};
        int massdistribution {0};
        int massradiusdistribution {0};
};

#endif // SPHEREGENERATOR_H
