#ifndef GRIDCONTROLLER_H
#define GRIDCONTROLLER_H


namespace gridcontroller
{
    void initialize();

    bool& getgridrender(int gridid);
    void setgridrender(int gridid, bool renderthis);
    void rendergrid();
};

#endif // GRIDCONTROLLER_H
