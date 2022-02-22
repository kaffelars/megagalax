#ifndef MASSIVEPARTICLEGENERATOR_H
#define MASSIVEPARTICLEGENERATOR_H

#include "objectsgenerator.h"

class massiveparticlegenerator : public objectsgenerator
{
    public:
        massiveparticlegenerator();
        virtual ~massiveparticlegenerator();
        void boxcontents();
        void generatenewparticles();

    protected:

    private:
        int smass {5000};
        glm::ivec3 sposition {glm::ivec3(0)};
        glm::ivec3 svelocity {glm::ivec3(0)};
};

#endif // MASSIVEPARTICLEGENERATOR_H
