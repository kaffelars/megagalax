#ifndef PARTICLECIRCLES_H
#define PARTICLECIRCLES_H

#include "shaderdata.h"

class particlecircles : public shaderdata
{
    public:
        particlecircles(shaderids shaderid);
        virtual ~particlecircles();

        void renderpanel();
        void senduniforms();

    protected:

    private:
        int circlesize {150};
        int opacity {50};
        int massdependence {20};
        int masscolor{20};
};

#endif // PARTICLECIRCLES_H
