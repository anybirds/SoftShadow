#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"

using namespace glm;

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
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render setting
    bool depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);
    GLint depth_test_mode;
    glGetIntegerv(GL_DEPTH_FUNC, &depth_test_mode);

    bool cull_face_enabled = glIsEnabled(GL_CULL_FACE);
    GLint cull_face_mode;
    glGetIntegerv(GL_CULL_FACE_MODE, &cull_face_mode);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    for (Renderer *rend : Renderer::renderers) {
        Mesh *mesh = rend->GetMesh();
        Material *material = rend->GetMaterial();

        glBindVertexArray(mesh->vao);

        glUseProgram(material->program);
        material->UseTextures();

        GLuint location;

        location = glGetUniformLocation(material->program, "_MODEL");

        mat4 _MODEL = rend->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_MODEL);

        location = glGetUniformLocation(material->program, "_CAM");
        mat4 _CAM = GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&_CAM);
        location = glGetUniformLocation(material->program, "_NORM");
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&normalization);

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