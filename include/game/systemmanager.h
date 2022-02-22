#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H


namespace systemmanager
{
    struct particlegroup
    {
        std::string name;
        uint32_t firstparticle;
        uint32_t finalparticle;
    };

    struct storagepoint
    {
        std::vector<glm::vec4> obj_posmasses;
        std::vector<glm::vec4> obj_velocities;

        std::vector<particlegroup> particlegroups; //to also restore particle groups

        bool has_data(){return obj_posmasses.size()>0;};
        int particlenumber(){return obj_posmasses.size();};
        void cleardata(){obj_posmasses.clear(); obj_velocities.clear();}
    };

    const std::vector<particlegroup>& getparticlegroups();
    void deleteparticlegroup(uint32_t id);
    void deleteeverynparticles(int deletechance);
    void deleteparticle(int id);

    void initialize();

    void clear_system();

    bool load_system(std::string filename);
    bool save_system(std::string name, std::string description, std::string filename);

    void unpause();
    void revertsystem(int fp);
    void storesystem(int fp);
    bool systemstored(int fp);
    void deletestoragepoint(int fp);
    int getstoragepointnumber();

    void download_current_obj_values();
    void download_current_obj_pos_values();
    void upload_current_obj_values();
    void bindssbos();

    void addparticle(glm::vec4 posmass, glm::vec4 velocity);

    int get_currentparticlenumber();
    uint64_t get_totalmass();

    void renderparticles();
    void addparticles(std::string source, std::vector<glm::vec4>& posmasses, std::vector<glm::vec4>& velocities);
    void sortparticles(glm::vec3 campos);
    void mergeparticles();

    void toggleparticlesorting();
    void setparticlesorting(bool on);

    //getters
    std::string& get_current_name();
    std::string& get_current_desc();
    bool get_particlesortingactive();


    void cleanup();
    //
};

#endif // SYSTEMMANAGER_H
