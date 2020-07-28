#include <EGL/egl.h>
#include <glm/gtx/rotate_vector.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "RotateScript.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include "NDKHelper.h"

using namespace glm;

extern EGLint width, height;

Scene::Scene() {
    GLenum err;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before scene : %d", err);
    }
    girlModel = new Model("girl.obj");
    girlMesh = new Mesh(girlModel);
    tileModel = new Model("tile.obj");
    tileMesh = new Mesh(tileModel);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before tex : %d", err);
    }
    girlTexture = new Texture("girl_diffuse.png");
    tileTexture = new Texture("tile_diffuse.jpg");
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before shader : %d", err);
    }
    litVertexShader = new Shader("lit_vert.glsl", GL_VERTEX_SHADER);
    litFragmentShader = new Shader("lit_frag.glsl", GL_FRAGMENT_SHADER);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before mat : %d", err);
    }
    girlMaterial = new Material(litVertexShader, litFragmentShader);
    girlMaterial->SetMainTexture(girlTexture);
    girlMaterial->SetVector("_AMBIENT", vec3(1.0f));
    girlMaterial->SetVector("_DIFFUSE", vec3(1.0f));
    girlMaterial->SetVector("_SPECULAR", vec3(1.0f));
    girlMaterial->SetFloat("_SHININESS", 25.0f);
    tileMaterial = new Material(litVertexShader, litFragmentShader);
    tileMaterial->SetMainTexture(tileTexture);
    tileMaterial->SetVector("_AMBIENT", vec3(1.0f));
    tileMaterial->SetVector("_DIFFUSE", vec3(1.0f));
    tileMaterial->SetVector("_SPECULAR", vec3(1.0f));
    tileMaterial->SetFloat("_SHININESS", 25.0f);

    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before cam : %d", err);
    }
    // mainCameraGameObject = new GameObject(vec3(0.0f, 10.0f, 0.0f), quat(radians(vec3(-90.0f, 0.0f, 0.0f))), vec3(1.0f));
    mainCameraGameObject = new GameObject(vec3(0.0f, 3.0f, 7.0f), quat(mat4(1.0f)), vec3(1.0f));
    mainCameraCamera = new Camera(perspective(radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f));
    mainCameraGameObject->AddComponent<Camera>(mainCameraCamera);
    Camera::SetMainCamera(mainCameraCamera);

    girlGameObject = new GameObject(vec3(0.0f), quat(radians(vec3(90.0f, 90.0f, -180.0f))), vec3(0.002f));
    girlRenderer = new Renderer(girlMaterial, girlMesh);
    girlGameObject->AddComponent<Renderer>(girlRenderer);
    girlRenderer->SetMaterial(girlMaterial);
    girlRenderer->SetMesh(girlMesh);
    girlRotateScript = new RotateScript();
    girlGameObject->AddComponent<RotateScript>(girlRotateScript);

    tileGameObject = new GameObject(vec3(0.0f), quat(mat4(1.0f)), vec3(5.0f));
    tileRenderer = new Renderer(tileMaterial, tileMesh);
    tileGameObject->AddComponent<Renderer>(tileRenderer);
    tileRenderer->SetMaterial(tileMaterial);
    tileRenderer->SetMesh(tileMesh);

    quat lightRotation = rotate(quat(mat4(1.0f)), radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    lightRotation = rotate(lightRotation, radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
    // quat lightRotation = rotate(quat(mat4(1.0f)), radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
    /*
    quat lightRotation = rotate(quat(mat4(1.0f)), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    lightRotation = rotate(lightRotation, radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
     */
    lightGameObject = new GameObject(vec3(10.0f), lightRotation, vec3(1.0f));
    lightLight = new Light(vec3(0.2f), vec3(1.0f), vec3(1.0f), vec2(0.05f));
    lightGameObject->AddComponent<Light>(lightLight);
    Light::SetMainLight(lightLight);
}

Scene::~Scene() {
    delete girlModel;
    delete girlTexture;
    delete litVertexShader;
    delete litFragmentShader;
    delete girlMaterial;
    delete girlGameObject;
    delete girlRenderer;
    delete girlMesh;
    delete mainCameraGameObject;
    delete mainCameraCamera;
    delete girlRotateScript;
    delete lightGameObject;
    delete lightLight;
}