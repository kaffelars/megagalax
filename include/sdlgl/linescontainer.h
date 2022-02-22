#ifndef LINESCONTAINER_H
#define LINESCONTAINER_H


class linescontainer
{
    public:
        linescontainer();
        virtual ~linescontainer();

        void cleanup();
        void initialize();
        void addline(glm::vec3 start, glm::vec3 startcolor, glm::vec3 stop, glm::vec3 stopcolor);
        void addline(glm::vec3 start, glm::vec3 stop, glm::vec3 color);
        void addline(glm::vec4 start, glm::vec4 stop);
        void clearlines();
        void setvbos();
        void render();

        float opacity;


    protected:

    private:
        GLuint pointsvbo, pointsvao;
        std::vector<glm::vec4> lines;
};

#endif // LINESCONTAINER_H
