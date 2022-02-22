#include "consts.h"
#include "systemmanager.h"
#include "shadercontroller.h"
#include "utils.h"
#include "simulationmanager.h"
#include "pugixml.hpp"

namespace systemmanager
{
    std::vector<glm::vec4> obj_posmasses (c_maxparticles, glm::vec4(0)); //xyz + mass, get from gpu when pause
    std::vector<glm::vec4> obj_velocities (c_maxparticles, glm::vec4(0));

    std::vector<storagepoint> storagepoints (2, storagepoint());//store points, 0 = first unpause, 1 = last unpause, 2+ = custom

    std::vector<glm::vec4> render_posmasses(c_maxparticles, glm::vec4(0));

    std::vector<glm::vec2> tempvector (c_maxparticles, glm::vec2(0));

    std::vector<particlegroup> particlegroups;

    std::vector<uint32_t> deleteparticles;

    bool firstunpause {false};

    bool sortparticlesactive {true};
    uint32_t currentparticlenumber {0};

    std::string current_name;
    std::string current_desc;

    int selected_obj;

    void updateparticlesvao();

    void deleteemptyparticlegroups();

    GLuint pointssbo, velssbo, tempvelssbo, pointsvbo, pointsvao, renderssbo;
}

namespace systemmanager
{
    std::string& get_current_name()
    {
        return current_name;
    }
    std::string& get_current_desc()
    {
        return current_desc;
    }
    bool get_particlesortingactive()
    {
        return sortparticlesactive;
    }
}

uint64_t systemmanager::get_totalmass()
{
    uint64_t totmass = 0;
    for (int a = 0; a < currentparticlenumber; a++)
    {
        totmass += obj_posmasses[a].a;
    }
    return totmass;
}

void systemmanager::mergeparticles()
{
    download_current_obj_values();

    deleteparticles.clear();


    //this may cause total mass to change a tiny bit due to float rounding fun
    for (int a = 0; a < currentparticlenumber; a++)
    {
        int m = std::round(obj_velocities[a].a);
        if (m >= 0.0f) //merging time
        {
            float curmass = obj_posmasses[a].a;
            float mmass = obj_posmasses[m].a;

            if (curmass > 0.0f && mmass > 0.0f)
            {
                float massfactor = mmass / (mmass + curmass);

                if (curmass >= mmass)
                {
                    obj_posmasses[a].a += mmass;
                    obj_posmasses[m].a = 0.0f;
                    obj_velocities[a] = obj_velocities[a] * (1.0f - massfactor) + obj_velocities[m] * massfactor;
                    deleteparticles.push_back(m);
                }
                else if (curmass < mmass)
                {
                    obj_posmasses[m].a += curmass;
                    obj_posmasses[a].a = 0.0f;
                    obj_velocities[m] = obj_velocities[a] * (1.0f - massfactor) + obj_velocities[m] * massfactor;
                    deleteparticles.push_back(a);
                }

                obj_velocities[m].a = -1.0f;
                obj_velocities[a].a = -1.0f;
            }
        }
    }

    std::sort(deleteparticles.begin(), deleteparticles.end());

    for (int a = deleteparticles.size()-1; a >= 0; a--)
    {
        deleteparticle(deleteparticles[a]);
    }

    upload_current_obj_values();
}

void systemmanager::unpause()
{
    if (!firstunpause)
    {
        storesystem(0);

        firstunpause = true;
    }

    storesystem(1);
}

bool systemmanager::systemstored(int fp)
{
    if (fp < storagepoints.size())
    {
        return storagepoints[fp].has_data();
    }
}

int systemmanager::getstoragepointnumber()
{
    return storagepoints.size();
}

void systemmanager::storesystem(int fp) //-1 = new storage point
{
    if (fp == -1)
    {
        storagepoints.push_back(storagepoint());
        fp = storagepoints.size() - 1;
    }

    if (fp < storagepoints.size())
    {
        storagepoints[fp].obj_posmasses.clear();
        storagepoints[fp].obj_velocities.clear();
        storagepoints[fp].particlegroups.clear();

        for (int a = 0; a < currentparticlenumber; a++)
        {
            storagepoints[fp].obj_posmasses.push_back(obj_posmasses[a]);
            storagepoints[fp].obj_velocities.push_back(obj_velocities[a]);
        }

        for (particlegroup& p : particlegroups)
        {
            storagepoints[fp].particlegroups.push_back(p);
        }
    }
}

void systemmanager::deletestoragepoint(int fp)
{
    if (fp > 1 && fp < storagepoints.size())
    {
        storagepoints.erase(storagepoints.begin() + fp);
    }
}

void systemmanager::revertsystem(int fp)
{
    if (fp >= 0 && fp < storagepoints.size())
    {
        if (storagepoints[fp].has_data())
        {
            for (int a = 0; a < storagepoints[fp].particlenumber(); a++)
            {
                obj_posmasses[a] = storagepoints[fp].obj_posmasses[a];
                obj_velocities[a] = storagepoints[fp].obj_velocities[a];
            }
            particlegroups.clear();
            for (particlegroup& p: storagepoints[fp].particlegroups)
            {
                particlegroups.push_back(p);
            }
            currentparticlenumber = storagepoints[fp].obj_posmasses.size();
        }
    }

}

void systemmanager::toggleparticlesorting()
{
    sortparticlesactive = !sortparticlesactive;
    upload_current_obj_values();
}

void systemmanager::setparticlesorting(bool on)
{
    sortparticlesactive = on;
    if (!on) upload_current_obj_values();
}

void systemmanager::initialize()
{
    //for rendering
    glGenBuffers(1, &pointsvbo);
    glGenVertexArrays(1, &pointsvao);
    glBindVertexArray(pointsvao);
    glBindBuffer(GL_ARRAY_BUFFER, pointsvbo);
    glBufferData(GL_ARRAY_BUFFER, 16*c_maxparticles, NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //for simulation
    glGenBuffers(1, &pointssbo); //position xyz + mass

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 16*c_maxparticles, NULL, GL_STATIC_DRAW); //dynamic draw?
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pointssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 16*c_maxparticles, &obj_posmasses[0]);

    glGenBuffers(1, &velssbo); //velocity xyz + ?

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, velssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 16*c_maxparticles, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 16*c_maxparticles, &obj_posmasses[0]);

    glGenBuffers(1, &tempvelssbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, tempvelssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 16*c_maxparticles, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, tempvelssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 16*c_maxparticles, &obj_posmasses[0]);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    //for rendering simulated particles directly
    glGenVertexArrays(1, &renderssbo);
    glBindVertexArray(renderssbo);
    glBindBuffer(GL_ARRAY_BUFFER, pointssbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}


void systemmanager::download_current_obj_values()
{
    //downloads current obj values from gpu
    //download kun positions?
    if (currentparticlenumber)
    {
        glGetNamedBufferSubData(pointssbo, 0, 16*currentparticlenumber, obj_posmasses.data());
        glGetNamedBufferSubData(velssbo, 0, 16*currentparticlenumber, obj_velocities.data());
    }
}

void systemmanager::download_current_obj_pos_values()
{
    //downloads current obj values from gpu
    //download only positions?
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    if (currentparticlenumber)
    {
        glGetNamedBufferSubData(pointssbo, 0, 16*currentparticlenumber, obj_posmasses.data());
        //glGetNamedBufferSubData(velssbo, 0, 16*currentparticlenumber, obj_velocities.data());
    }
}

void systemmanager::upload_current_obj_values() //
{
    //uploads current obj values to gpu shader storage buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pointssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 16*c_maxparticles, &obj_posmasses[0]);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 16*c_maxparticles, &obj_velocities[0]);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void systemmanager::bindssbos()//neccessary? no
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pointssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velssbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, tempvelssbo);
}


void systemmanager::clear_system()
{
    currentparticlenumber = 0;

    particlegroups.clear();

    storagepoints[0].cleardata();
    storagepoints[1].cleardata();

    while (getstoragepointnumber() > 2)
    {
        deletestoragepoint(storagepoints.size() - 1);
    }

    firstunpause = false;

    current_name = "Default name";
    current_desc = "No description";
}

void systemmanager::deleteeverynparticles(int n)
{
    int deleted = 0;
    std::vector<uint32_t> particlestodelete;
    for (int a = 0; a < currentparticlenumber; a++)
    {
        if (a%n == 0)
        {
            particlestodelete.push_back(a);
        }
    }

    for (int a = particlestodelete.size()-1; a >= 0; a=a-1)
    {
        deleteparticle(particlestodelete[a]);
    }

    deleteemptyparticlegroups();
}

void systemmanager::deleteparticle(int id)
{
    for (int a = id; a < currentparticlenumber-1; a++)
    {
        obj_posmasses[a] = obj_posmasses[a + 1];
        obj_velocities[a] = obj_velocities[a + 1];
    }

    for (particlegroup& p:particlegroups)
    {
        if (p.firstparticle > id)
        {
            p.firstparticle --;
        }
        if (p.finalparticle > id)
        {
            p.finalparticle --;
        }
    }

    currentparticlenumber --;
}

void systemmanager::deleteemptyparticlegroups()
{
    //remove any empty particlegroups
    auto it = particlegroups.begin();
    while (it != particlegroups.end())
    {
        if (it->finalparticle == it->firstparticle)
        {
            it = particlegroups.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void systemmanager::deleteparticlegroup(uint32_t id)
{
    uint32_t particlestodelete = particlegroups[id].finalparticle - particlegroups[id].firstparticle;

    for (int a = particlegroups[id].firstparticle; a < currentparticlenumber-particlestodelete; a++)
    {
        obj_posmasses[a] = obj_posmasses[a + particlestodelete];
        obj_velocities[a] = obj_velocities[a + particlestodelete];
    }

    for (particlegroup& p:particlegroups)
    {
        if (p.firstparticle > particlegroups[id].firstparticle)
        {
            p.firstparticle -= particlestodelete;
            p.finalparticle -= particlestodelete;
        }
    }

    currentparticlenumber -= particlestodelete;

    particlegroups.erase(particlegroups.begin() + id);
}

const std::vector<systemmanager::particlegroup>& systemmanager::getparticlegroups()
{
    return particlegroups;
}


void systemmanager::updateparticlesvao()
{
    glBindVertexArray(pointsvao);

    glBindBuffer(GL_ARRAY_BUFFER, pointsvbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, currentparticlenumber * 16, &render_posmasses[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void systemmanager::sortparticles(glm::vec3 campos)
{
    //sorts all particles back to front relative to campos - only for rendering
    tempvector.clear();

    for (int a = 0; a < currentparticlenumber; a++)
    {
        glm::vec3 temp_objpos = glm::vec3(obj_posmasses[a].x, obj_posmasses[a].y, obj_posmasses[a].z);
        tempvector.push_back(glm::vec2(glm::length(temp_objpos-campos), a));
    }

    std::sort(tempvector.begin(), tempvector.end(), [](glm::vec2& a, glm::vec2& b)->bool
    {
        return a.x > b.x;
    });

    int index = 0;

    for (glm::vec2 info : tempvector)
    {
        render_posmasses[index] = obj_posmasses[info.y];
        index++;
    }

    updateparticlesvao();
}

void systemmanager::renderparticles()
{
    if (currentparticlenumber)
    {
        if (sortparticlesactive)
        {
            glBindVertexArray(pointsvao);
            glDrawArrays(GL_POINTS, 0, currentparticlenumber);
            glBindVertexArray(0);
        }
        else
        {
            glBindVertexArray(renderssbo);
            glDrawArrays(GL_POINTS, 0, currentparticlenumber);
            glBindVertexArray(0);
        }
    }
}

void systemmanager::addparticles(std::string source, std::vector<glm::vec4>& posmasses, std::vector<glm::vec4>& velocities)//, std::vector<float>& masses)
{
    particlegroup p;

    p.name = source;
    p.firstparticle = currentparticlenumber;

    for (int a = 0; a < posmasses.size(); a++)
    {
        systemmanager::addparticle(posmasses[a], velocities[a]);
    }

    p.finalparticle = currentparticlenumber; //actually final particle +1

    particlegroups.push_back(p);

    upload_current_obj_values();

    updateparticlesvao();
}

void systemmanager::addparticle(glm::vec4 posmass, glm::vec4 velocity)
{
    if (currentparticlenumber < c_maxparticles)
    {
        obj_posmasses[currentparticlenumber] = posmass;
        obj_velocities[currentparticlenumber] = velocity;
        currentparticlenumber ++;
    }
}

int systemmanager::get_currentparticlenumber()
{
    return currentparticlenumber;
}

void systemmanager::cleanup()
{
    glDeleteBuffers(1, &pointsvbo);
    glDeleteVertexArrays(1, &pointsvao);
}

bool systemmanager::save_system(std::string name, std::string description, std::string filename)
{
    pugi::xml_document doc;

    pugi::xml_node node = doc.append_child("info");
    node.append_attribute("name") = name.c_str();
    node.append_attribute("description") = description.c_str();
    node.append_attribute("version") = program_version;
    node.append_attribute("particles") = get_currentparticlenumber();

    pugi::xml_node sim = doc.append_child("simulation");
    sim.append_attribute("gravity") = simulationmanager::getcurrentgravity();
    sim.append_attribute("slowdown") = simulationmanager::getcurrentslowdown();

    int mode = simulationmanager::getcomputeshadervalue(shaderids::compute_gravity, 4);

    sim.append_attribute("mode") = mode;

    if (mode == 1)
    {
        sim.append_attribute("veltransfer") = simulationmanager::getcomputeshadervalue(shaderids::compute_gravity, 1);
        sim.append_attribute("masstransfer") = simulationmanager::getcomputeshadervalue(shaderids::compute_gravity, 2);
    }
    if (mode != 0)
    {
        sim.append_attribute("interactiondistance") = simulationmanager::getcomputeshadervalue(shaderids::compute_gravity, 3);
    }

    pugi::xml_node particles = doc.append_child("particledata");

    for (int a = 0; a < currentparticlenumber; a++)
    {
        pugi::xml_node pnode = particles.append_child("particle");
        pnode.append_attribute("posx") = obj_posmasses[a].x;
        pnode.append_attribute("posy") = obj_posmasses[a].y;
        pnode.append_attribute("posz") = obj_posmasses[a].z;
        pnode.append_attribute("mass") = obj_posmasses[a].a;

        pnode.append_attribute("velx") = obj_velocities[a].x;
        pnode.append_attribute("vely") = obj_velocities[a].y;
        pnode.append_attribute("velz") = obj_velocities[a].z;
        pnode.append_attribute("rer") = obj_velocities[a].a;
    }

    std::string file = systemsavefolder+filename+".txt";
    return doc.save_file(file.c_str());
}

bool systemmanager::load_system(std::string filename)
{
    clear_system();

    std::string fullfilename = systemsavefolder + filename;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fullfilename.c_str());

    current_name = doc.child("info").attribute("name").value();
    current_desc = doc.child("info").attribute("description").value();
    currentparticlenumber = doc.child("info").attribute("particles").as_int();

    simulationmanager::setcurrentgravity(doc.child("simulation").attribute("gravity").as_int());
    simulationmanager::setcurrentslowdown(doc.child("simulation").attribute("slowdown").as_int());

    int mode = doc.child("simulation").attribute("mode").as_int();

    simulationmanager::setcomputeshadervalue(shaderids::compute_gravity, 4, mode);

    if (mode == 1)
    {
        simulationmanager::setcomputeshadervalue(shaderids::compute_gravity, 1, doc.child("simulation").attribute("veltransfer").as_int());
        simulationmanager::setcomputeshadervalue(shaderids::compute_gravity, 2, doc.child("simulation").attribute("masstransfer").as_int());
    }
    if (mode != 0)
    {
        simulationmanager::setcomputeshadervalue(shaderids::compute_gravity, 3, doc.child("simulation").attribute("interactiondistance").as_int());
    }

    pugi::xml_node particledata = doc.child("particledata");

    int i = 0;

    for (pugi::xml_node p = particledata.child("particle"); p; p = p.next_sibling("particle"))
    {
            obj_posmasses[i].x = p.attribute("posx").as_float();
            obj_posmasses[i].y = p.attribute("posy").as_float();
            obj_posmasses[i].z = p.attribute("posz").as_float();
            obj_posmasses[i].a = p.attribute("mass").as_float();

            obj_velocities[i].x = p.attribute("velx").as_float();
            obj_velocities[i].y = p.attribute("vely").as_float();
            obj_velocities[i].z = p.attribute("velz").as_float();
            obj_velocities[i].a = p.attribute("rer").as_float();
            i++;
    }

    return true;
}
