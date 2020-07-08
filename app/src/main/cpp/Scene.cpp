#include <EGL/egl.h>

#include "Camera.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

using namespace glm;

extern EGLint width, height;

Scene::Scene() {
    triModel = new Model("triangle.obj");
    triMesh = new Mesh(triModel);
    triTexture = new Texture("brick.jpg");
    unlitVertexShader = new Shader("unlit_vert.glsl", GL_VERTEX_SHADER);
    unlitFragmentShader = new Shader("unlit_frag.glsl", GL_FRAGMENT_SHADER);
    triMaterial = new Material(unlitVertexShader, unlitFragmentShader);
    triMaterial->SetMainTexture(triTexture);

    mainCameraGameObject = new GameObject(vec3(0.0f, 0.0f, 3.0f), mat4(1.0f), vec3(1.0f));
    // how to get screen width and height?
    mainCameraCamera = new Camera(perspective(radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f));
    mainCameraGameObject->AddComponent<Camera>(mainCameraCamera);
    Camera::SetMainCamera(mainCameraCamera);

    triGameObject = new GameObject(vec3(0.0f), mat4(1.0f), vec3(1.0f));
    triRenderer = new Renderer(triMaterial, triMesh);
    triGameObject->AddComponent<Renderer>(triRenderer);
    triRenderer->SetMaterial(triMaterial);
    triRenderer->SetMesh(triMesh);
}

Scene::~Scene() {
    delete triModel;
    delete triTexture;
    delete unlitVertexShader;
    delete unlitFragmentShader;
    delete triMaterial;
    delete triGameObject;
    delete triRenderer;
    delete triMesh;
    delete mainCameraGameObject;
    delete mainCameraCamera;
}