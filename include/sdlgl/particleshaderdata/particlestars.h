#ifndef PARTICLESTARS_H
#define PARTICLESTARS_H


#include "shaderdata.h"

class particlestars : public shaderdata
{
    public:
        particlestars(shaderids shaderid);
        virtual ~particlestars();

        void renderpanel();
        void senduniforms();

    protected:

    private:
        int opacity {100};
        int starsize {50};
};

#endif // PARTICLESTARS_H
