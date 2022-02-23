#include "consts.h"
#include "utils.h"
#include "messagelogger.h"

namespace utils
{
    float packu2f(uint8_t a, uint8_t b, uint8_t c)
    {
        uint32_t pack = (a << 16) | (b << 8) | c;
        return ((double)pack) / ((double) (1 << 24));
    }

    float getpackednorm(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3)
    {
        glm::vec3 v1 = p2 - p1; glm::vec3 v2 = p3 - p1;
        glm::vec3 norm = glm::normalize(glm::cross(v1, v2));
        return packnormal(norm);
    }

    float packnormal(glm::vec3 normal) //må være normalisert
    {
        normal += glm::vec3(1.0f);
        normal /= 2.0f;

        return packu2f(normal.x*255, normal.y*255, normal.z * 255);
    }

    float packnormal(float x, float y, float z)
    {
        return packnormal(glm::vec3(x,y,z));
    }

    float packrgb(glm::vec3 rgb)
    {
        return packu2f(rgb.x*255, rgb.y*255, rgb.z * 255);
    }

    float packuvtex(float uva, float uvb, uint8_t texid)
    {
        return packu2f(uva*255, uvb*255, texid);
    }

    void replaceAll(std::string& source, const std::string& from, const std::string& to)
    {
        //https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
        std::string newString;
        newString.reserve(source.length());

        std::string::size_type lastPos = 0;
        std::string::size_type findPos;

        while(std::string::npos != (findPos = source.find(from, lastPos)))
        {
            newString.append(source, lastPos, findPos - lastPos);
            newString += to;
            lastPos = findPos + from.length();
        }

        newString.append(source, lastPos, source.length() - lastPos);

        source.swap(newString);
    }
}
