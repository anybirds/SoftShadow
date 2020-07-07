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

Scene::Scene() {
    triModel = new Model("triangle.obj");
    triMesh = new Mesh(triModel);
    triTexture = new Texture("brick.jpg");
    unlitVertexShader = new Shader("unlit_vert.glsl", GL_VERTEX_SHADER);
    unlitFragmentShader = new Shader("unlit_frag.glsl", GL_FRAGMENT_SHADER);
    triMaterial = new Material(unlitVertexShader, unlitFragmentShader);
    triMaterial->SetMainTexture(triTexture);

    mainCameraGameObject = new GameObject(vec3(0.0f, 0.0f, 5.0f), mat4(1.0f), vec3(1.0f));
    // how to get screen width and height?
    mainCameraCamera = new Camera(mainCameraGameObject, perspective(radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f));
    mainCameraGameObject->AddComponent<Camera>(mainCameraCamera);
    Camera::SetMainCamera(mainCameraCamera);

    triGameObject = new GameObject(vec3(0.0f), mat4(1.0f), vec3(1.0f));
    triRenderer = new Renderer(triGameObject, triMaterial, triMesh);
    triGameObject->AddComponent<Renderer>(triRenderer);
    triRenderer->SetMaterial(triMaterial);
    triRenderer->SetMesh(triMesh);
}