#ifndef LIGHT_H
#define LIGHT_H

#include <unordered_set>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include "Component.h"

#define SHADOW_MAP_WIDTH 1024
#define SHADOW_MAP_HEIGHT 1024
#define HSM_MAX_LEVEL 8
#define LIGHT_SIZE 5.0f

class Material;
class Shader;

class Light : public Component {
private:
    static Light *mainLight;
    static Shader *shadowMapVertexShader;
    static Shader *shadowMapFragmentShader;
    static Material *shadowMapMaterial;

    static Shader *hsmVertexShader;
    static Shader *hsmFragmentShader;
    static Shader *hsmBaseFragmentShader;
    static Material *hsmMaterial;
    static Material *hsmBaseMaterial;
    static GLuint emptyVao;

public:
    static Light *GetMainLight() { return mainLight; }
    static void SetMainLight(Light *mainLight) { Light::mainLight = mainLight; }
    static void Init();

private:
    GLuint shadowMapFBO;
    GLuint shadowMap;

    GLuint hsmFBO[HSM_MAX_LEVEL];
    GLuint hsm;

    glm::mat4 normalization;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec2 area;

public:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec2 &area);
    virtual ~Light();

    void RenderShadowMap();
    void RenderHSM();

    friend class Camera;
    friend class Material;
};

#endif