#ifndef PARTICLEGASCLOUDS_H
#define PARTICLEGASCLOUDS_H

#include "shaderdata.h"

class particlegasclouds : public shaderdata
{
    public:
        particlegasclouds(shaderids shaderid);
        virtual ~particlegasclouds();

        void renderpanel();
        void senduniforms();

    protected:

    private:
        int cloudsize {850};
        int opacity {50};
};

#endif // PARTICLEGASCLOUDS_H
