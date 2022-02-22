#include "consts.h"
#include "systemsinfomanager.h"

#include "pugixml.hpp"

namespace systemsinfomanager
{
    std::vector<system_info> systems_info;
}

const std::vector<systemsinfomanager::system_info>& systemsinfomanager::get_systems_info()
{
    return systems_info;
}

void systemsinfomanager::load_systems_info()
{
    DIR *dpdf;
    dirent *epdf;

    systems_info.clear();

    dpdf = opendir(systemsavefolder.c_str());
    if (dpdf != NULL){
        while (epdf = readdir(dpdf)){
            system_info s;
            s.filename = epdf->d_name;
            if (s.filename != "" && s.filename != "." && s.filename != "..")
                systems_info.push_back(s);
        }
    }
    closedir(dpdf);

    for (system_info& s : systems_info)
    {
        std::string fullfilename = systemsavefolder + s.filename;
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(fullfilename.c_str());

        if (result)
        {
            s.name = doc.child("info").attribute("name").value();
            s.description = doc.child("info").attribute("description").value();
            s.particlenumber = doc.child("info").attribute("particles").as_int();
        }
        else
        {
            s.name = "error: "+s.filename;
        }
    }
}
