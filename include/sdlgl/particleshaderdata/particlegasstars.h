#ifndef PARTICLEGASSTARS_H
#define PARTICLEGASSTARS_H


#include "shaderdata.h"

class particlegasstars : public shaderdata
{
    public:
        particlegasstars(shaderids shaderid);
        virtual ~particlegasstars();

        void renderpanel();
        void senduniforms();

    protected:

    private:
        int cloudsize {850};
        int opacity {50};
        int starsize {50};
        int starmass {500};
};

#endif // PARTICLEGASSTARS_H
