#include <cmath>
#include <EGL/egl.h>

#include "GameObject.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "Transform.h"
#include "Time.h"

#include "NDKHelper.h"

using namespace std;
using namespace glm;

extern EGLint width, height;

const GLenum bufs[1] = { GL_NONE };

Light *Light::mainLight;
Shader *Light::shadowMapVertexShader;
Shader *Light::shadowMapFragmentShader;
Material *Light::shadowMapMaterial;

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
    GLenum err;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before light : %d", err);
    }
    // generate shadow map framebuffer
    glGenFramebuffers(1, &shadowMapFBO);

    // generate shadow map texture
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // generate hsm framebuffer
    glGenFramebuffers(HSM_MAX_LEVEL + 1, hsmFBO);

    // generate hsm texture
    glGenTextures(1, &hsm);
    glBindTexture(GL_TEXTURE_2D, hsm);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, HSM_MAX_LEVEL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    for (int i=0; i<=HSM_MAX_LEVEL; i++) {
        glTexImage2D(GL_TEXTURE_2D, i, GL_RG16F, SHADOW_MAP_WIDTH >> i, SHADOW_MAP_HEIGHT >> i, 0, GL_RG, GL_FLOAT, NULL);
    }

    // generate vsm framebuffer
    glGenFramebuffers(2, vsmFBO);

    // generate vsm texture
    glGenTextures(2, vsmTemp);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before vsm : %d", err);
    }
    for (int i=0; i<2; i++) {
        glBindTexture(GL_TEXTURE_2D, vsmTemp[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // no GL_CLAMP_TO_BORDER defined, need to implement in fragment shader
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, SHADOW_MAP_WIDTH + 2, SHADOW_MAP_HEIGHT + 2, 0, GL_RG, GL_FLOAT, NULL); // clamp to [0, 1] does not occur in the fragment shader with this format
    }

    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror after vsm : %d", err);
    }

    // attach shadow map texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffers(1, bufs);
    glReadBuffer(GL_NONE);

    // attach hsm textures to corresponding framebuffer
    for (int i=0; i<HSM_MAX_LEVEL; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, hsmFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hsm, i);
    }

    // attach vsm textures to corresponding framebuffer
    for (int i=0; i<2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vsmTemp[i], 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set normalization matrix
    float ratio = (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT;
    normalization = ortho(-ratio * LIGHT_SIZE, ratio * LIGHT_SIZE, -LIGHT_SIZE, LIGHT_SIZE, 0.0f, 40.0f);
}

Light::~Light() {

}

void Light::RenderShadowMap() {
    Time::StartTimer();

    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
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
    glCullFace(GL_BACK);

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

    float elapsed = Time::StopTimer();
    LOGI("Rendering shadow map took %f seconds", elapsed);
}

void Light::RenderHSM() {
    Time::StartTimer();

    // render hsm base level
    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, hsmFBO[0]);
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(emptyVao);
    glUseProgram(hsmBaseMaterial->program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render hsm
    glBindTexture(GL_TEXTURE_2D, hsm);
    for (int i=1; i<=HSM_MAX_LEVEL; i++) {
        glViewport(0, 0, SHADOW_MAP_WIDTH >> i, SHADOW_MAP_HEIGHT >> i);
        glBindFramebuffer(GL_FRAMEBUFFER, hsmFBO[i]);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(emptyVao);
        glUseProgram(hsmMaterial->program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hsm);
        hsmMaterial->SetInteger("_LEVEL", i - 1);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float elapsed = Time::StopTimer();
    LOGI("Rendering HSM took %f seconds", elapsed);
}

void Light::RenderVSM() {
    Time::StartTimer();

    // render vsm base
    glViewport(0, 0, SHADOW_MAP_WIDTH + 2, SHADOW_MAP_HEIGHT + 2);
    glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[0]);
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(emptyVao);
    glUseProgram(vsmBaseMaterial->program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render vsm
    int n = (int)(ceil(log2((float)SHADOW_MAP_WIDTH) / 2.0f));
    int m = (int)(ceil(log2((float)SHADOW_MAP_HEIGHT) / 2.0f));

    for (int i=0; i<n; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[(i + 1) % 2]);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(emptyVao);
        glUseProgram(vsmMaterial->program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vsmTemp[i % 2]);
        vsmMaterial->SetVector("_STRIDE", vec2(pow(4, i), 0.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    for (int i=n; i<n+m; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, vsmFBO[(i + 1) % 2]);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(emptyVao);
        glUseProgram(vsmMaterial->program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vsmTemp[i % 2]);
        vsmMaterial->SetVector("_STRIDE", vec2(0.0f, pow(4, i - n)));
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // store average depth value
    /*
    float sum[16];
    glReadPixels(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, SHADOW_MAP_WIDTH + 2, SHADOW_MAP_HEIGHT + 2, GL_RGBA, GL_FLOAT, sum);
    averageDepth = sum[0] / (SHADOW_MAP_WIDTH * SHADOW_MAP_HEIGHT);
     */

/*
    GLenum err;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror: %d", err);
    }
    float vsmPixels[34][34][4];
    glReadPixels(0, 0, SHADOW_MAP_WIDTH + 2, SHADOW_MAP_HEIGHT + 2, GL_RGBA, GL_FLOAT, vsmPixels);
    LOGI("start");
    for (int i=1; i<=32; i++) {
        LOGI("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
                vsmPixels[i][1][0],
             vsmPixels[i][2][0],
             vsmPixels[i][3][0],
             vsmPixels[i][4][0],
             vsmPixels[i][5][0],
             vsmPixels[i][6][0],
             vsmPixels[i][7][0],
             vsmPixels[i][8][0],
             vsmPixels[i][9][0],
             vsmPixels[i][10][0],
             vsmPixels[i][11][0],
             vsmPixels[i][12][0],
             vsmPixels[i][13][0],
             vsmPixels[i][14][0],
             vsmPixels[i][15][0],
             vsmPixels[i][16][0],
             vsmPixels[i][17][0],
             vsmPixels[i][18][0],
             vsmPixels[i][19][0],
             vsmPixels[i][20][0],
             vsmPixels[i][21][0],
             vsmPixels[i][22][0],
             vsmPixels[i][23][0],
             vsmPixels[i][24][0],
             vsmPixels[i][25][0],
             vsmPixels[i][26][0],
             vsmPixels[i][27][0],
             vsmPixels[i][28][0],
             vsmPixels[i][29][0],
             vsmPixels[i][30][0],
             vsmPixels[i][31][0],
             vsmPixels[i][32][0]
                );
    }
    LOGI("end");
    */
    // store the final result as vsm
    vsm = vsmTemp[(n + m) % 2];

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float elapsed = Time::StopTimer();
    LOGI("Rendering VSM took %f seconds", elapsed);
}