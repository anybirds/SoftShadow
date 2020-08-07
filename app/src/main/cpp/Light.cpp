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

void Light::Init() {
    shadowMapVertexShader = new Shader("shadow_map_vert.glsl", GL_VERTEX_SHADER);
    shadowMapFragmentShader = new Shader("shadow_map_frag.glsl", GL_FRAGMENT_SHADER);
    shadowMapMaterial = new Material(shadowMapVertexShader, shadowMapFragmentShader);
}

Light::Light(const vec3 &ambient, const vec3 &diffuse, const vec3 &specular, const vec2 &area) : ambient(ambient), diffuse(diffuse), specular(specular), area(area) {
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

    // attach shadow map texture to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffers(1, bufs);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set normalization matrix
    float ratio = (float)SHADOW_MAP_WIDTH / (float)SHADOW_MAP_HEIGHT;
    normalization = ortho(-ratio * LIGHT_SIZE, ratio * LIGHT_SIZE, -LIGHT_SIZE, LIGHT_SIZE, 0.0f, 50.0f);
}

Light::~Light() {
    // should free all gles resources
}

void Light::RenderShadowMap() {
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
    glCullFace(GL_FRONT);

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
}