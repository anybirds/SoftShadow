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
#include "Slider.h"

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
    litFragmentShader[0] = new Shader("lit_frag_pcf.glsl", GL_FRAGMENT_SHADER);
    litFragmentShader[1] = new Shader("lit_frag_pcss.glsl", GL_FRAGMENT_SHADER);
    litFragmentShader[2] = new Shader("lit_frag_vssm.glsl", GL_FRAGMENT_SHADER);
    for (int i=0; i<3; i++) {
        girlMaterial[i] = new Material(litVertexShader, litFragmentShader[i]);
        girlMaterial[i]->SetMainTexture(girlTexture);
        girlMaterial[i]->SetVector("_AMBIENT", vec3(1.0f));
        girlMaterial[i]->SetVector("_DIFFUSE", vec3(1.0f));
        girlMaterial[i]->SetVector("_SPECULAR", vec3(1.0f));
        girlMaterial[i]->SetFloat("_SHININESS", 25.0f);
        tileMaterial[i] = new Material(litVertexShader, litFragmentShader[i]);
        tileMaterial[i]->SetMainTexture(tileTexture);
        tileMaterial[i]->SetVector("_AMBIENT", vec3(1.0f));
        tileMaterial[i]->SetVector("_DIFFUSE", vec3(1.0f));
        tileMaterial[i]->SetVector("_SPECULAR", vec3(1.0f));
        tileMaterial[i]->SetFloat("_SHININESS", 25.0f);
    }

    quat cameraRotation = rotate(quat(mat4(1.0f)), radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));
    mainCameraGameObject = new GameObject(vec3(-6.0f, 3.0f, 6.0f), cameraRotation, vec3(1.0f));
    // mainCameraGameObject = new GameObject(vec3(0.0f, 15.0f, 0.0f), quat(radians(vec3(-90.0f, 0.0f, 0.0f))), vec3(1.0f));
    // mainCameraGameObject = new GameObject(vec3(0.0f, 3.0f, 7.0f), quat(mat4(1.0f)), vec3(1.0f));
    mainCameraCamera = new Camera(perspective(radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f));
    mainCameraGameObject->AddComponent<Camera>(mainCameraCamera);
    Camera::SetMainCamera(mainCameraCamera);

    girlGameObject = new GameObject(vec3(0.0f), quat(radians(vec3(90.0f, 90.0f, -180.0f))), vec3(0.002f));
    girlRenderer = new Renderer(girlMaterial[0], girlMesh);
    girlGameObject->AddComponent<Renderer>(girlRenderer);
    girlRotateScript = new RotateScript();
    girlGameObject->AddComponent<RotateScript>(girlRotateScript);

    tileGameObject = new GameObject(vec3(0.0f), quat(mat4(1.0f)), vec3(5.0f));
    tileRenderer = new Renderer(tileMaterial[0], tileMesh);
    tileGameObject->AddComponent<Renderer>(tileRenderer);

    // quat lightRotation = rotate(quat(mat4(1.0f)), radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
    // lightRotation = rotate(lightRotation, radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
    quat lightRotation = rotate(quat(mat4(1.0f)), radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));

    /*
    quat lightRotation = rotate(quat(mat4(1.0f)), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    lightRotation = rotate(lightRotation, radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
     */
    lightGameObject = new GameObject(vec3(0.0, 10.0f, 10.0f), lightRotation, vec3(1.0f));
    lightLight = new Light(vec3(0.2f), vec3(1.0f), vec3(1.0f), vec2(0.03f));
    lightGameObject->AddComponent<Light>(lightLight);
    Light::SetMainLight(lightLight);

    panelTexture = new Texture("white_img.jpg");
    panelGameObject = new GameObject(vec3(width - 300.0f, height - 650.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    panelPanel = new Panel(panelTexture, vec2(300.0f, 650.0f), vec4(0.5f, 0.5f, 0.5f, 0.5f), 0);
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

    algorithmTextGameObject = new GameObject(vec3(width - 280.0f, height - 275.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmTextText = new Text("# Shadow Algorithm", arialFont, vec4(1.0f), 1);
    algorithmTextGameObject->AddComponent<Text>(algorithmTextText);

    algorithmOptionButtonGameObject[0] = new GameObject(vec3(width - 280.0f, height - 350.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmOptionButtonButton[0] = new Button(buttonTexture, vec2(80.0f, 50.0f), vec4(1.0f), 1);
    algorithmOptionButtonGameObject[0]->AddComponent<Button>(algorithmOptionButtonButton[0]);
    algorithmOptionButtonGameObject[1] = new GameObject(vec3(width - 190.0f, height - 350.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmOptionButtonButton[1] = new Button(buttonTexture, vec2(80.0f, 50.0f), vec4(1.0f), 1);
    algorithmOptionButtonGameObject[1]->AddComponent<Button>(algorithmOptionButtonButton[1]);
    algorithmOptionButtonGameObject[2] = new GameObject(vec3(width - 100.0f, height - 350.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmOptionButtonButton[2] = new Button(buttonTexture, vec2(80.0f, 50.0f), vec4(1.0f), 1);
    algorithmOptionButtonGameObject[2]->AddComponent<Button>(algorithmOptionButtonButton[2]);

    algorithmOptionTextGameObject[0] = new GameObject(vec3(width - 265.0f, height - 335.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmOptionTextText[0] = new Text("PCF", arialFont, vec4(1.0f), 2);
    algorithmOptionTextGameObject[0]->AddComponent<Text>(algorithmOptionTextText[0]);
    algorithmOptionTextGameObject[1] = new GameObject(vec3(width - 185.0f, height - 335.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmOptionTextText[1] = new Text("PCSS", arialFont, vec4(1.0f), 2);
    algorithmOptionTextGameObject[1]->AddComponent<Text>(algorithmOptionTextText[1]);
    algorithmOptionTextGameObject[2] = new GameObject(vec3(width - 95.0f, height - 335.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    algorithmOptionTextText[2] = new Text("VSSM", arialFont, vec4(1.0f), 2);
    algorithmOptionTextGameObject[2]->AddComponent<Text>(algorithmOptionTextText[2]);

    areaTextGameObject = new GameObject(vec3(width - 280.0f, height - 400.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    areaTextText = new Text("# Area Light Size", arialFont, vec4(1.0f), 1);
    areaTextGameObject->AddComponent<Text>(areaTextText);

    handleTexture = new Texture("circle_img.png");
    areaSliderGameObject = new GameObject(vec3(width - 260.0f, height - 475.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    areaSliderSlider = new Slider(buttonTexture, handleTexture, vec2(220.0f, 40.0f), 10.0f, vec4(0.0f, 0.5f, 1.0f, 1.0f), vec4(1.0f), 1);
    areaSliderGameObject->AddComponent<Slider>(areaSliderSlider);

    viewTextGameObject = new GameObject(vec3(width - 280.0f, height - 525.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    viewTextText = new Text("# View", arialFont, vec4(1.0f), 1);
    viewTextGameObject->AddComponent<Text>(viewTextText);

    viewOptionButtonGameObject[0] = new GameObject(vec3(width - 280.0f, height - 600.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    viewOptionButtonButton[0] = new Button(buttonTexture, vec2(120.0f, 50.0f), vec4(1.0f), 1);
    viewOptionButtonGameObject[0]->AddComponent<Button>(viewOptionButtonButton[0]);
    viewOptionButtonGameObject[1] = new GameObject(vec3(width - 140.0f, height - 600.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    viewOptionButtonButton[1] = new Button(buttonTexture, vec2(120.0f, 50.0f), vec4(1.0f), 1);
    viewOptionButtonGameObject[1]->AddComponent<Button>(viewOptionButtonButton[1]);
    viewOptionTextGameObject[0] = new GameObject(vec3(width - 245.0f, height - 585.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    viewOptionTextText[0] = new Text("Side", arialFont, vec4(1.0f), 2);
    viewOptionTextGameObject[0]->AddComponent<Text>(viewOptionTextText[0]);
    viewOptionTextGameObject[1] = new GameObject(vec3(width - 100.0f, height - 585.0f, 0.0f), quat(mat4(1.0f)), vec3(1.0f));
    viewOptionTextText[1] = new Text("Top", arialFont, vec4(1.0f), 2);
    viewOptionTextGameObject[1]->AddComponent<Text>(viewOptionTextText[1]);

    statusScript = new StatusScript(fpsTextText,
            sizeOptionButtonButton[0], sizeOptionButtonButton[1], sizeOptionTextText[0], sizeOptionTextText[1],
            algorithmOptionButtonButton[0], algorithmOptionButtonButton[1], algorithmOptionButtonButton[2], algorithmOptionTextText[0], algorithmOptionTextText[1], algorithmOptionTextText[2],
            girlRenderer, tileRenderer, girlMaterial[0], girlMaterial[1], girlMaterial[2], tileMaterial[0], tileMaterial[1], tileMaterial[2],
            areaSliderSlider,
            viewOptionButtonButton[0], viewOptionButtonButton[1], viewOptionTextText[0], viewOptionTextText[1]);
}

Scene::~Scene() {
    // should delete all the members
}