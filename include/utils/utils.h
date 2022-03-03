#ifndef UTILS_H
#define UTILS_H

//diverse utility functions

namespace utils
{
    float packu2f(uint8_t a, uint8_t b, uint8_t c);

    float packnormal(glm::vec3 normal); //må være normalisert
    float packnormal(float x, float y, float z);

    float packrgb(glm::vec3 rgb);

    float getpackednorm(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3);

    std::string get_programversionstring();
    std::string get_programversionstring(int programversion);

    void replaceAll(std::string& source, const std::string& from, const std::string& to);
}


#endif // UTILS_H
