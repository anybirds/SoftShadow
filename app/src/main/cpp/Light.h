#ifndef LIGHT_H
#define LIGHT_H

#include <unordered_set>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

#include "Component.h"

#define HSM_MAX_LEVEL 8
#define LIGHT_SIZE 7.5f

class Material;
class Shader;

class Light : public Component {
private:
    static Light *mainLight;
    static Shader *shadowMapVertexShader;
    static Shader *shadowMapFragmentShader;
    static Material *shadowMapMaterial;

    static int shadowMapSize[2];

    static GLuint emptyVao;
    static Shader *quadVertexShader;

    static Shader *hsmFragmentShader;
    static Shader *hsmBaseFragmentShader;
    static Material *hsmMaterial;
    static Material *hsmBaseMaterial;

    static Shader *vsmFragmentShader;
    static Shader *vsmBaseFragmentShader;
    static Material *vsmMaterial;
    static Material *vsmBaseMaterial;

public:
    static Light *GetMainLight() { return mainLight; }
    static void SetMainLight(Light *mainLight) { Light::mainLight = mainLight; }
    static void Init();

private:
    int shadowMapSizeOption;
    bool vssm;

    GLuint shadowMapFBO[2];
    GLuint shadowMap[2];

    GLuint hsmFBO[2][HSM_MAX_LEVEL];
    GLuint hsm[2];

    GLuint vsmFBO[2][2];
    GLuint vsmTemp[2][2];
    GLuint vsm[2];

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

    void SetShadowAlgorithmPCF() { vssm = false; }
    void SetShadowAlgorithmPCSS() { vssm = false; }
    void SetShadowAlgorithmVSSM() { vssm = true; }

    void SetAreaSize(const glm::vec2 &area) { this->area = area; }

    void RenderShadowMap();
    void RenderHSM();
    void RenderVSM();

    friend class Camera;
    friend class Material;
};

#endif