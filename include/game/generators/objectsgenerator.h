#ifndef OBJECTSGENERATOR_H
#define OBJECTSGENERATOR_H


class objectsgenerator
{
    public:
        objectsgenerator();
        virtual ~objectsgenerator();

        void renderbox();

        void startbox();
        virtual void boxcontents() {};
        void endbox();

        float uiscale;
        glm::vec2 position;
        std::string title;


        void rendertempparticles();
        void updatevao();
        bool readytoapply();
        int gettotalmass();

        int particlenumber {1};

        bool hidebox {false};

        std::vector<glm::vec4> gen_posmasses;
        std::vector<glm::vec4> gen_velocities;
        //std::vector<float> obj_masses;

        void updategeneration();
        virtual void generatenewparticles() {};

        void cleardata();

    protected:

    private:
};

#endif // OBJECTSGENERATOR_H
