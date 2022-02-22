#ifndef COMPUTEMOVE_H
#define COMPUTEMOVE_H

#include "shaderdata.h"

class computemove : public shaderdata
{
    public:
        computemove(shaderids shaderid);
        virtual ~computemove();

        void renderpanel();
        void senduniforms();

        int getvalue(int valueid);
        void setvalue(int valueid, int value);

    protected:

    private:

        int slowdown {75};
};

#endif // COMPUTEMOVE_H
