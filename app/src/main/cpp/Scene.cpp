#include <EGL/egl.h>

#include "Camera.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "RotateScript.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

using namespace glm;

extern EGLint width, height;

Scene::Scene() {
    girlModel = new Model("girl.obj");
    girlMesh = new Mesh(girlModel);
    girlTexture = new Texture("girl_diffuse.png");
    unlitVertexShader = new Shader("unlit_vert.glsl", GL_VERTEX_SHADER);
    unlitFragmentShader = new Shader("unlit_frag.glsl", GL_FRAGMENT_SHADER);
    girlMaterial = new Material(unlitVertexShader, unlitFragmentShader);
    girlMaterial->SetMainTexture(girlTexture);

    mainCameraGameObject = new GameObject(vec3(0.0f, 0.0f, 5.0f), mat4(1.0f), vec3(1.0f));
    // how to get screen width and height?
    mainCameraCamera = new Camera(perspective(radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f));
    mainCameraGameObject->AddComponent<Camera>(mainCameraCamera);
    Camera::SetMainCamera(mainCameraCamera);

    girlGameObject = new GameObject(vec3(0.0f, -2.0f, 0.0f), quat(radians(vec3(90.0f, 90.0f, -180.0f))), vec3(0.002f));
    girlRenderer = new Renderer(girlMaterial, girlMesh);
    girlGameObject->AddComponent<Renderer>(girlRenderer);
    girlRenderer->SetMaterial(girlMaterial);
    girlRenderer->SetMesh(girlMesh);
    girlRotateScript = new RotateScript();
    girlGameObject->AddComponent<RotateScript>(girlRotateScript);
}

Scene::~Scene() {
    delete girlModel;
    delete girlTexture;
    delete unlitVertexShader;
    delete unlitFragmentShader;
    delete girlMaterial;
    delete girlGameObject;
    delete girlRenderer;
    delete girlMesh;
    delete mainCameraGameObject;
    delete mainCameraCamera;
    delete girlRotateScript;
}