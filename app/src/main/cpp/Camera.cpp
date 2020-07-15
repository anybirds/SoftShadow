#include <EGL/egl.h>

#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Transform.h"

#include "NDKHelper.h"

using namespace glm;

extern EGLint width, height;

Camera *Camera::mainCamera;

Camera *Camera::GetMainCamera() {
    return mainCamera;
}

void Camera::SetMainCamera(Camera *mainCamera) {
    Camera::mainCamera = mainCamera;
}

Camera::Camera(const glm::mat4 &normalization) : normalization(normalization) {

}

Camera::~Camera() {

}

void Camera::Render() {
    Light *light = Light::GetMainLight();
    light->RenderShadowMap();
    light->RenderHSM();

    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        Material *material = rend->GetMaterial();

        glBindVertexArray(mesh->vao);

        glUseProgram(material->program);
        material->UseTextures();

        mat4 _MODEL = rend->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        mat4 _CAM = GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        mat4 _NORM = normalization;
        mat4 _NVM = inverse(_MODEL);
        _NVM[0][3] = 0.0f; _NVM[1][3] = 0.0f; _NVM[2][3] = 0.0f;
        _NVM = transpose(_NVM);
        mat4 _MVP = _NORM * inverse(_CAM) * _MODEL;

        material->SetMatrix("_MODEL", _MODEL);
        material->SetMatrix("_CAM", _CAM);
        material->SetMatrix("_NORM", _NORM);
        material->SetMatrix("_NVM", _NVM);
        material->SetMatrix("_MVP", _MVP);

        material->SetVector("_LIGHT._AMBIENT", light->ambient);
        material->SetVector("_LIGHT._DIFFUSE", light->diffuse);
        material->SetVector("_LIGHT._SPECULAR", light->specular);
        mat3 _LIGHT_ORIENTATION = toMat3(light->GetGameObject()->GetTransform()->GetRotation());
        vec3 _LIGHT_DIR = _LIGHT_ORIENTATION[2];
        material->SetVector("_LIGHT._DIR", _LIGHT_DIR);
        mat4 _LIGHT = light->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        mat4 _LIGHT_WLP = light->normalization * inverse(_LIGHT);
        material->SetMatrix("_LIGHT._WLP", _LIGHT_WLP);

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
}