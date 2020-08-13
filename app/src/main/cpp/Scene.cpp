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
#include "Font.h"
#include "Text.h"
#include "Button.h"
#include "Panel.h"
#include "Time.h"
#include "StatusScript.h"

#include "NDKHelper.h"

using namespace std;
using namespace glm;

extern EGLint width, height;

Scene::Scene() {
    girlModel = new Model("girl.obj");
    girlMesh = new Mesh(girlModel);
    tileModel = new Model("tile.obj");
    tileMesh = new Mesh(tileModel);
    girlTexture = new Texture("girl_diffuse.png");
    tileTexture = new Texture("tile_diffuse.jpg");
    litVertexShader = new Shader("lit_vert.glsl", GL_VERTEX_SHADER);
    litFragmentShader = new Shader("lit_frag.glsl", GL_FRAGMENT_SHADER);
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

    quat cameraRotation = rotate(quat(mat4(1.0f)), radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));
    mainCameraGameObject = new GameObject(vec3(-6.0f, 3.0f, 6.0f), cameraRotation, vec3(1.0f));
    // mainCameraGameObject = new GameObject(vec3(0.0f, 15.0f, 0.0f), quat(radians(vec3(-90.0f, 0.0f, 0.0f))), vec3(1.0f));
    // mainCameraGameObject = new GameObject(vec3(0.0f, 3.0f, 7.0f), quat(mat4(1.0f)), vec3(1.0f));
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

    // quat lightRotation = rotate(quat(mat4(1.0f)), radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    // lightRotation = rotate(lightRotation, radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
    quat lightRotation = rotate(quat(mat4(1.0f)), radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));

    /*
    quat lightRotation = rotate(quat(mat4(1.0f)), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    lightRotation = rotate(lightRotation, radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
     */
    lightGameObject = new GameObject(vec3(0.0, 10.0f, 10.0f), lightRotation, vec3(1.0f));
    lightLight = new Light(vec3(0.2f), vec3(1.0f), vec3(1.0f), vec2(0.05f));
    lightGameObject->AddComponent<Light>(lightLight);
    Light::SetMainLight(lightLight);

    panelTexture = new Texture("white_img.jpg");
    panelGameObject = new GameObject(vec3(width - 300.0f, height - 300.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    panelPanel = new Panel(panelTexture, vec2(300.0f, 300.0f), vec4(0.5f, 0.5f, 0.5f, 0.5f), 0);
    panelGameObject->AddComponent<Panel>(panelPanel);

    arialFont = new Font("arial.ttf");
    fpsTextGameObject = new GameObject(vec3(width - 280.0f, height - 100.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    fpsTextText = new Text("", arialFont, vec4(1.0f), 1);
    fpsTextGameObject->AddComponent<Text>(fpsTextText);

    sizeTextGameObject = new GameObject(vec3(width - 280.0f, height - 150.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    sizeTextText = new Text("# Shadow Map Size", arialFont, vec4(1.0f), 1);
    sizeTextGameObject->AddComponent<Text>(sizeTextText);

    buttonTexture = new Texture("white_img.jpg");
    sizeOptionButtonGameObject[0] = new GameObject(vec3(width - 280.0f, height - 225.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    sizeOptionButtonButton[0] = new Button(buttonTexture, vec2(120.0f, 50.0f), vec4(1.0f), 1);
    sizeOptionButtonGameObject[0]->AddComponent<Button>(sizeOptionButtonButton[0]);
    sizeOptionButtonGameObject[1] = new GameObject(vec3(width - 140.0f, height - 225.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    sizeOptionButtonButton[1] = new Button(buttonTexture, vec2(120.0f, 50.0f), vec4(1.0f), 1);
    sizeOptionButtonGameObject[1]->AddComponent<Button>(sizeOptionButtonButton[1]);
    sizeOptionTextGameObject[0] = new GameObject(vec3(width - 265.0f, height - 210.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    sizeOptionTextText[0] = new Text("512*512", arialFont, vec4(1.0f), 2);
    sizeOptionTextGameObject[0]->AddComponent<Text>(sizeOptionTextText[0]);
    sizeOptionTextGameObject[1] = new GameObject(vec3(width - 135.0f, height - 210.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    sizeOptionTextText[1] = new Text("1024*1024", arialFont, vec4(1.0f), 2);
    sizeOptionTextGameObject[1]->AddComponent<Text>(sizeOptionTextText[1]);

    statusScript = new StatusScript(fpsTextText, sizeTextText, sizeOptionButtonButton[0], sizeOptionButtonButton[1], sizeOptionTextText[0], sizeOptionTextText[1]);
}

Scene::~Scene() {
    // should delete all the members
}