#ifndef LIGHT_H
#define LIGHT_H

#include <unordered_set>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include "Component.h"

#define LIGHT_SIZE 10.0f

class Material;
class Shader;

class Light : public Component {
private:
    static Light *mainLight;
    static Shader *shadowMapVertexShader;
    static Shader *shadowMapFragmentShader;
    static Material *shadowMapMaterial;

    static int shadowMapSize[2];

public:
    static Light *GetMainLight() { return mainLight; }
    static void SetMainLight(Light *mainLight) { Light::mainLight = mainLight; }
    static void Init();

private:
    int shadowMapSizeOption;
    GLuint shadowMapFBO[2];
    GLuint shadowMap[2];

    glm::mat4 normalization;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec2 area;

public:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec2 &area);
    virtual ~Light();

    void SetShadowMapSizeSmall() { shadowMapSizeOption = 0; }
    void SetShadowMapSizeBig() { shadowMapSizeOption = 1; }

    void RenderShadowMap();

    friend class Camera;
    friend class Material;
};

#endif