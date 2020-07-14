#ifndef LIGHT_H
#define LIGHT_H

#include <unordered_set>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include "Component.h"

class Material;

class Light : public Component {
private:
    static Light *mainLight;
    static Material *shadowMapMaterial;

public:
    static Light *GetMainLight() { return mainLight; }
    static void SetMainLight(Light *mainLight) { Light::mainLight = mainLight; }
    static Material *GetShadowMapMaterial() { return shadowMapMaterial; }
    static void SetShadowMapMaterial(Material *shadowMapMaterial) { Light::shadowMapMaterial = shadowMapMaterial; }

private:
    GLuint fbo;
    GLuint shadowMap;
    glm::mat4 normalization;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    virtual ~Light();

    void RenderShadowMap();

    friend class Camera;
    friend class Material;
};

#endif