#ifndef PARTICLESHADERPANEL_H
#define PARTICLESHADERPANEL_H


class shaderdata
{
    public:
        shaderdata(shaderids shaderid);
        virtual ~shaderdata();

        virtual void renderpanel() {};
        virtual void senduniforms() {};

        std::string particleshadername;
        shaderids thisshaderid;

        virtual int getvalue(int valueid) {return 0;};
        virtual void setvalue(int valueid, int value) {};

    protected:

    private:
};

#endif // PARTICLESHADERPANEL_H
