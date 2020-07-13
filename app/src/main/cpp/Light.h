#ifndef LIGHT_H
#define LIGHT_H

#include <unordered_set>
#include <glm/glm.hpp>

#include "Component.h"

class Light : public Component {
private:
    static Light *mainLight;

public:
    static Light *GetMainLight() { return mainLight; }
    static void SetMainLight(Light *light) { mainLight = light; }

private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    virtual ~Light();

    friend class Camera;
};

#endif