#ifndef COMPUTEDEFAULT_H
#define COMPUTEDEFAULT_H

#include "shaderdata.h"

class computedefault : public shaderdata
{
    public:
        computedefault(shaderids shaderid);
        virtual ~computedefault();

        void renderpanel();
        void senduniforms();

        int getvalue(int valueid);
        void setvalue(int valueid, int value);

    protected:

    private:
        int shadermode {0};
        int gravity {500};
        int veltransfer {0};
        int masstransfer {0};
        int interactiondistance {10};
};

#endif // COMPUTEDEFAULT_H
