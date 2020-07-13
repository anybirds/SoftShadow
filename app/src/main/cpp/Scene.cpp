#include <EGL/egl.h>

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

using namespace glm;

extern EGLint width, height;

Scene::Scene() {
    girlModel = new Model("girl.obj");
    girlMesh = new Mesh(girlModel);
    girlTexture = new Texture("girl_diffuse.png");
    litVertexShader = new Shader("lit_vert.glsl", GL_VERTEX_SHADER);
    litFragmentShader = new Shader("lit_frag.glsl", GL_FRAGMENT_SHADER);
    girlMaterial = new Material(litVertexShader, litFragmentShader);
    girlMaterial->SetMainTexture(girlTexture);
    girlMaterial->SetVector("_AMBIENT", vec3(1.0f));
    girlMaterial->SetVector("_DIFFUSE", vec3(1.0f));
    girlMaterial->SetVector("_SPECULAR", vec3(1.0f));
    girlMaterial->SetFloat("_SHININESS", 25.0f);

    mainCameraGameObject = new GameObject(vec3(0.0f, 0.0f, 5.0f), quat(mat4(1.0f)), vec3(1.0f));
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

    quat lightRotation = rotate(quat(mat4(1.0f)), radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
    lightRotation = rotate(lightRotation, radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    lightGameObject = new GameObject(vec3(0.0f), lightRotation, vec3(1.0f));
    lightLight = new Light(vec3(0.2f), vec3(1.0f), vec3(1.0f, 1.0f, 1.0f));
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