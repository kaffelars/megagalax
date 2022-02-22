#ifndef CURRENTGENERATORPOINTS_H
#define CURRENTGENERATORPOINTS_H


namespace currentgeneratorpoints
{
    void initialize();
    void updatevao(std::vector<glm::vec4>& points, std::vector<glm::vec4>& velocities);
    void render();
    void cleanup();

    void toggleshowvelocities(bool turnon);
    bool isshowingvelocities();
};

#endif // CURRENTGENERATORPOINTS_H
