#ifndef SYSTEMSMANAGER_H
#define SYSTEMSMANAGER_H


namespace systemsinfomanager
{
    struct system_info
    {
        std::string name;
        std::string filename;
        std::string description;
        int particlenumber;
    };

    void load_systems_info();
    const std::vector<system_info>& get_systems_info();
};

#endif // SYSTEMSMANAGER_H
