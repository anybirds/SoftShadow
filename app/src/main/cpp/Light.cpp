#include <cmath>
#include <EGL/egl.h>

#include "GameObject.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Transform.h"

#include "NDKHelper.h"

using namespace std;
using namespace glm;

extern EGLint width, height;

const GLenum bufs[1] = { GL_NONE };

Light *Light::mainLight;
Shader *Light::shadowMapVertexShader;
Shader *Light::shadowMapFragmentShader;
Material *Light::shadowMapMaterial;

int Light::shadowMapSize[2];

GLuint Light::emptyVao;
Shader *Light::quadVertexShader;

Shader *Light::hsmFragmentShader;
Shader *Light::hsmBaseFragmentShader;
Material *Light::hsmMaterial;
Material *Light::hsmBaseMaterial;

Shader *Light::vsmFragmentShader;
Shader *Light::vsmBaseFragmentShader;
Material *Light::vsmMaterial;
Material *Light::vsmBaseMaterial;

void Light::Init() {
    shadowMapVertexShader = new Shader("shadow_map_vert.glsl", GL_VERTEX_SHADER);
    shadowMapFragmentShader = new Shader("shadow_map_frag.glsl", GL_FRAGMENT_SHADER);
    shadowMapMaterial = new Material(shadowMapVertexShader, shadowMapFragmentShader);

    shadowMapSize[0] = 512;
    shadowMapSize[1] = 1024;

    // empty vao used to draw full-screen quad
    glGenVertexArrays(1, &emptyVao);
    quadVertexShader = new Shader("quad_vert.glsl", GL_VERTEX_SHADER);

    hsmFragmentShader = new Shader("hsm_frag.glsl", GL_FRAGMENT_SHADER);
    hsmBaseFragmentShader = new Shader("hsm_base_frag.glsl", GL_FRAGMENT_SHADER);
    hsmMaterial = new Material(quadVertexShader, hsmFragmentShader);
    hsmMaterial->SetInteger("_HSM", 0);
    hsmBaseMaterial = new Material(quadVertexShader, hsmBaseFragmentShader);
    hsmBaseMaterial->SetInteger("_SHADOW_MAP", 0);

    vsmFragmentShader = new Shader("vsm_frag.glsl", GL_FRAGMENT_SHADER);
    vsmBaseFragmentShader = new Shader("vsm_base_frag.glsl", GL_FRAGMENT_SHADER);
    vsmMaterial = new Material(quadVertexShader, vsmFragmentShader);
    vsmMaterial->SetInteger("_VSM", 0);
    vsmBaseMaterial = new Material(quadVertexShader, vsmBaseFragmentShader);
    vsmBaseMaterial->SetInteger("_SHADOW_MAP", 0);
}

Light::Light(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular, const vec2 &area) : ambient(ambient), diffuse(diffuse), specular(specular), area(area) {
    shadowMapSizeOption = 0;
    vssm = false;

    glGenFramebuffers(2, shadowMapFBO);
    glGenTextures(2, shadowMap);

    glGenFramebuffers(HSM_MAX_LEVEL + 1, hsmFBO[0]);
    glGenFramebuffers(HSM_MAX_LEVEL + 1, hsmFBO[1]);
    glGenTextures(2, hsm);

    glGenFramebuffers(2, vsmFBO[0]);
    glGenFramebuffers(2, vsmFBO[1]);
    glGenTextures(2, vsmTemp[0]);
    glGenTextures(2, vsmTemp[1]);

    for (int i=0; i<2; i++) {
        glBindTexture(GL_TEXTURE_2D, shadowMap[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowMapSize[i], shadowMapSize[i], 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, hsm[i]);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, HSM_MAX_LEVEL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        for (int j=0; j<=HSM_MAX_LEVEL; j++) {
            glTexImage2D(GL_TEXTURE_2D, j, GL_RG16F, shadowMapSize[i] >> j, shadowMapSize[i] >> j, 0, GL_RG, GL_FLOAT, NULL);
        }

        for (int j=0; j<2; j++) {
            glBindTexture(GL_TEXTURE_2D, vsmTemp[i][j]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // no GL_CLAMP_TO_BORDER defined, need to implement in fragment shader
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32I, shadowMapSize[i], shadowMapSize[i], 0, GL_RG_INTEGER, GL_INT, NULL); // clamp to [0, 1] does not occur in the fragment shader with this format
        }

        // attach shadow map texture to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap[i], 0);
        glDrawBuffers(1, bufs);
        glReadBuffer(GL_NONE);

        // attach hsm textures to corresponding framebuffer
        for (int j=0; j<HSM_MAX_LEVEL; j++) {
            glBindFramebuffer(GL_FRAMEBUFFER, hsmFBO[i][j]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hsm[i], j);
        }

        // attach vsm textures to corresponding framebuffer
        for (int j=0; j<2; j++) {
            glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[i][j]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vsmTemp[i][j], 0);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set normalization matrix
    normalization = ortho(-LIGHT_SIZE, LIGHT_SIZE, -LIGHT_SIZE, LIGHT_SIZE, 0.0f, 50.0f);
}

Light::~Light() {
    // should free all gles resources
}

void Light::RenderShadowMap() {
    glViewport(0, 0, shadowMapSize[shadowMapSizeOption], shadowMapSize[shadowMapSizeOption]);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO[shadowMapSizeOption]);
    glClearDepthf(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);

    // render setting
    bool depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);
    GLint depth_test_mode;
    glGetIntegerv(GL_DEPTH_FUNC, &depth_test_mode);

    bool cull_face_enabled = glIsEnabled(GL_CULL_FACE);
    GLint cull_face_mode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cull_face_mode);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    if (vssm) {
        glCullFace(GL_BACK);
    } else {
        glCullFace(GL_FRONT);
    }

    for (Renderer *rend : Renderer::renderers) {
        Mesh *mesh = rend->GetMesh();
        Material *material = shadowMapMaterial;

        glBindVertexArray(mesh->vao);

        glUseProgram(material->program);

        mat4 _MODEL = rend->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        mat4 _CAM = GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        mat4 _NORM = normalization;
        mat4 _MVP = _NORM * inverse(_CAM) * _MODEL;

        material->SetMatrix("_MVP", _MVP);

        if (!mesh->icnt) {
            // mesh without EBO
            glDrawArrays(GL_TRIANGLES, 0, mesh->vcnt);
        }
        else {
            // mesh with EBO
            glDrawElements(GL_TRIANGLES, mesh->icnt, GL_UNSIGNED_INT, 0);
        }
    }

    // render setting
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthFunc(depth_test_mode);

    if (cull_face_enabled) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    glCullFace(cull_face_mode);

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (vssm) {
        RenderHSM();
        RenderVSM();
    }
}


void Light::RenderHSM() {
    // render hsm base level
    glViewport(0, 0, shadowMapSize[shadowMapSizeOption], shadowMapSize[shadowMapSizeOption]);
    glBindFramebuffer(GL_FRAMEBUFFER, hsmFBO[shadowMapSizeOption][0]);
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(emptyVao);
    glUseProgram(hsmBaseMaterial->program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap[shadowMapSizeOption]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render hsm
    glBindTexture(GL_TEXTURE_2D, hsm[shadowMapSizeOption]);
    for (int i=1; i<=HSM_MAX_LEVEL; i++) {
        glViewport(0, 0, shadowMapSize[shadowMapSizeOption] >> i, shadowMapSize[shadowMapSizeOption] >> i);
        glBindFramebuffer(GL_FRAMEBUFFER, hsmFBO[shadowMapSizeOption][i]);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(emptyVao);
        glUseProgram(hsmMaterial->program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hsm[shadowMapSizeOption]);
        hsmMaterial->SetInteger("_LEVEL", i - 1);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::RenderVSM() {
    // render vsm base
    glViewport(0, 0, shadowMapSize[shadowMapSizeOption], shadowMapSize[shadowMapSizeOption]);
    glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[shadowMapSizeOption][0]);
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(emptyVao);
    glUseProgram(vsmBaseMaterial->program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap[shadowMapSizeOption]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render vsm
    int n = (int)(ceil(log2((float)shadowMapSize[shadowMapSizeOption]) / 2.0f));
    int m = (int)(ceil(log2((float)shadowMapSize[shadowMapSizeOption]) / 2.0f));

    for (int i=0; i<n; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[shadowMapSizeOption][(i + 1) % 2]);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(emptyVao);
        glUseProgram(vsmMaterial->program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vsmTemp[shadowMapSizeOption][i % 2]);
        vsmMaterial->SetVector("_STRIDE", vec2(pow(4, i), 0.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    for (int i=n; i<n+m; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[shadowMapSizeOption][(i + 1) % 2]);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(emptyVao);
        glUseProgram(vsmMaterial->program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vsmTemp[shadowMapSizeOption][i % 2]);
        vsmMaterial->SetVector("_STRIDE", vec2(0.0f, pow(4, i - n)));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // store the final result as vsm
    vsm[shadowMapSizeOption] = vsmTemp[shadowMapSizeOption][(n + m) % 2];

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}