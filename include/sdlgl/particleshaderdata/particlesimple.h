#ifndef PARTICLESIMPLE_H
#define PARTICLESIMPLE_H

#include "shaderdata.h"

class particlesimple : public shaderdata
{
    public:
        particlesimple(shaderids shaderid);
        virtual ~particlesimple();

        void renderpanel();
        void senduniforms();

    protected:

    private:
        int particlesize {2};
};

#endif // PARTICLESIMPLE_H
