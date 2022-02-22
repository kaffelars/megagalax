#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H


namespace simulationmanager
{
    void toggle_pause_simulation();
    void pause_simulation();
    void unpause_simulation();
    bool is_simulation_paused();

    void simulate();
    void initialize();
    int getcurrentgravity();
    int getcurrentslowdown();
    int getcomputeshadervalue(shaderids shaderid, int valueid);
    void setcomputeshadervalue(shaderids shaderid, int valueid, int value);
    void setcurrentgravity(int grav);
    void setcurrentslowdown(int sdown);
};

#endif // SIMULATIONMANAGER_H
