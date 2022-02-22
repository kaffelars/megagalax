#ifndef SINGLEPARTICLEGENERATOR_H
#define SINGLEPARTICLEGENERATOR_H

#include "objectsgenerator.h"

class singleparticlegenerator : public objectsgenerator
{
    public:
        singleparticlegenerator();
        virtual ~singleparticlegenerator();

        void boxcontents();
        void generatenewparticles();

    protected:

    private:
        int smass {5000};
        glm::ivec3 sposition {glm::ivec3(0)};
        glm::ivec3 svelocity {glm::ivec3(0)};
};

#endif // SINGLEPARTICLEGENERATOR_H
