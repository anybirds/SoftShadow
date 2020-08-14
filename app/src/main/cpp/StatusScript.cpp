#include "StatusScript.h"
#include "Text.h"
#include "Time.h"
#include "Button.h"
#include "Light.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"
#include "Slider.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

using namespace std;
using namespace glm;

vec4 StatusScript::unselectedButtonColor = vec4(1.0f);
vec4 StatusScript::selectedButtonColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);
vec4 StatusScript::unselectedTextColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 StatusScript::selectedTextColor = vec4(1.0f);
glm::vec3 StatusScript::topViewPosition = vec3(0.0f, 15.0f, 0.0f);
glm::quat StatusScript::topViewRotation = quat(radians(vec3(-90.0f, 0.0f, 0.0f)));
glm::vec3 StatusScript::sideViewPosition = vec3(-6.0f, 3.0f, 6.0f);
glm::quat StatusScript::sideViewRotation = rotate(quat(mat4(1.0f)), radians(-45.0f), vec3(0.0f, 1.0f, 0.0f));

StatusScript::StatusScript(Text *fpsText,
        Button *shadowMapSizeOptionButton0, Button *shadowMapSizeOptionButton1, Text *shadowMapSizeOptionText0, Text *shadowMapSizeOptionText1,
        Button *shadowAlgorithmOptionButton0, Button *shadowAlgorithmOptionButton1, Button *shadowAlgorithmOptionButton2, Text *shadowAlgorithmOptionText0, Text *shadowAlgorithmOptionText1, Text *shadowAlgorithmOptionText2,
        Renderer *girlRenderer, Renderer *tileRenderer, Material *girlMaterial0, Material *girlMaterial1, Material *girlMaterial2, Material *tileMaterial0, Material *tileMaterial1, Material *tileMaterial2,
        Slider *areaSlider,
        Button *viewOptionButton0, Button *viewOptionButton1, Text *viewOptionText0, Text *viewOptionText1)
        : fpsText(fpsText), girlRenderer(girlRenderer), tileRenderer(tileRenderer), areaSlider(areaSlider) {
    shadowMapSizeOptionButton[0] = shadowMapSizeOptionButton0;
    shadowMapSizeOptionButton[1] = shadowMapSizeOptionButton1;
    shadowMapSizeOptionText[0] = shadowMapSizeOptionText0;
    shadowMapSizeOptionText[1] = shadowMapSizeOptionText1;

    shadowMapSizeOptionButton[0]->RegisterOnPressEvent(this, OnPressShadowMapSizeOptionButton0);
    shadowMapSizeOptionButton[1]->RegisterOnPressEvent(this, OnPressShadowMapSizeOptionButton1);

    shadowMapSizeOptionButton[0]->SetColor(selectedButtonColor);
    shadowMapSizeOptionButton[1]->SetColor(unselectedButtonColor);
    shadowMapSizeOptionText[0]->SetColor(selectedTextColor);
    shadowMapSizeOptionText[1]->SetColor(unselectedTextColor);

    Light::GetMainLight()->SetShadowMapSizeSmall();

    shadowAlgorithmOptionButton[0] = shadowAlgorithmOptionButton0;
    shadowAlgorithmOptionButton[1] = shadowAlgorithmOptionButton1;
    shadowAlgorithmOptionButton[2] = shadowAlgorithmOptionButton2;
    shadowAlgorithmOptionText[0] = shadowAlgorithmOptionText0;
    shadowAlgorithmOptionText[1] = shadowAlgorithmOptionText1;
    shadowAlgorithmOptionText[2] = shadowAlgorithmOptionText2;

    shadowAlgorithmOptionButton[0]->RegisterOnPressEvent(this, OnPressShadowAlgorithmOptionButton0);
    shadowAlgorithmOptionButton[1]->RegisterOnPressEvent(this, OnPressShadowAlgorithmOptionButton1);
    shadowAlgorithmOptionButton[2]->RegisterOnPressEvent(this, OnPressShadowAlgorithmOptionButton2);

    shadowAlgorithmOptionButton[0]->SetColor(selectedButtonColor);
    shadowAlgorithmOptionText[0]->SetColor(selectedTextColor);
    shadowAlgorithmOptionButton[1]->SetColor(unselectedButtonColor);
    shadowAlgorithmOptionText[1]->SetColor(unselectedTextColor);
    shadowAlgorithmOptionButton[2]->SetColor(unselectedButtonColor);
    shadowAlgorithmOptionText[2]->SetColor(unselectedTextColor);

    girlMaterial[0] = girlMaterial0;
    girlMaterial[1] = girlMaterial1;
    girlMaterial[2] = girlMaterial2;
    tileMaterial[0] = tileMaterial0;
    tileMaterial[1] = tileMaterial1;
    tileMaterial[2] = tileMaterial2;

    girlRenderer->SetMaterial(girlMaterial[0]);
    tileRenderer->SetMaterial(tileMaterial[0]);

    viewOptionButton[0] = viewOptionButton0;
    viewOptionButton[1] = viewOptionButton1;
    viewOptionText[0] = viewOptionText0;
    viewOptionText[1] = viewOptionText1;

    viewOptionButton[0]->RegisterOnPressEvent(this, OnPressViewOptionButton0);
    viewOptionButton[1]->RegisterOnPressEvent(this, OnPressViewOptionButton1);

    viewOptionButton[0]->SetColor(selectedButtonColor);
    viewOptionButton[1]->SetColor(unselectedButtonColor);
    viewOptionText[0]->SetColor(selectedTextColor);
    viewOptionText[1]->SetColor(unselectedTextColor);

    Transform *transform = Camera::GetMainCamera()->GetGameObject()->GetTransform();
    transform->SetPosition(sideViewPosition);
    transform->SetLocalRotation(sideViewRotation);
}

StatusScript::~StatusScript() {

}

void StatusScript::OnUpdate() {
    int fps = Time::Fps();
    string text = "# FPS: ";
    text += to_string(fps);
    fpsText->SetString(text);

    Light::GetMainLight()->SetAreaSize(vec2(0.01f + 0.04f * areaSlider->GetValue()));
}

void StatusScript::OnPressShadowMapSizeOptionButton0(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowMapSizeOptionButton[0]->SetColor(selectedButtonColor);
    statusScript->shadowMapSizeOptionButton[1]->SetColor(unselectedButtonColor);
    statusScript->shadowMapSizeOptionText[0]->SetColor(selectedTextColor);
    statusScript->shadowMapSizeOptionText[1]->SetColor(unselectedTextColor);

    Light::GetMainLight()->SetShadowMapSizeSmall();
}

void StatusScript::OnPressShadowMapSizeOptionButton1(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowMapSizeOptionButton[0]->SetColor(unselectedButtonColor);
    statusScript->shadowMapSizeOptionButton[1]->SetColor(selectedButtonColor);
    statusScript->shadowMapSizeOptionText[0]->SetColor(unselectedTextColor);
    statusScript->shadowMapSizeOptionText[1]->SetColor(selectedTextColor);

    Light::GetMainLight()->SetShadowMapSizeBig();
}

void StatusScript::OnPressShadowAlgorithmOptionButton0(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowAlgorithmOptionButton[0]->SetColor(selectedButtonColor);
    statusScript->shadowAlgorithmOptionText[0]->SetColor(selectedTextColor);
    statusScript->shadowAlgorithmOptionButton[1]->SetColor(unselectedButtonColor);
    statusScript->shadowAlgorithmOptionText[1]->SetColor(unselectedTextColor);
    statusScript->shadowAlgorithmOptionButton[2]->SetColor(unselectedButtonColor);
    statusScript->shadowAlgorithmOptionText[2]->SetColor(unselectedTextColor);

    statusScript->girlRenderer->SetMaterial(statusScript->girlMaterial[0]);
    statusScript->tileRenderer->SetMaterial(statusScript->tileMaterial[0]);
    Light::GetMainLight()->SetShadowAlgorithmPCF();
}

void StatusScript::OnPressShadowAlgorithmOptionButton1(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowAlgorithmOptionButton[0]->SetColor(unselectedButtonColor);
    statusScript->shadowAlgorithmOptionText[0]->SetColor(unselectedTextColor);
    statusScript->shadowAlgorithmOptionButton[1]->SetColor(selectedButtonColor);
    statusScript->shadowAlgorithmOptionText[1]->SetColor(selectedTextColor);
    statusScript->shadowAlgorithmOptionButton[2]->SetColor(unselectedButtonColor);
    statusScript->shadowAlgorithmOptionText[2]->SetColor(unselectedTextColor);

    statusScript->girlRenderer->SetMaterial(statusScript->girlMaterial[1]);
    statusScript->tileRenderer->SetMaterial(statusScript->tileMaterial[1]);
    Light::GetMainLight()->SetShadowAlgorithmPCSS();
}

void StatusScript::OnPressShadowAlgorithmOptionButton2(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowAlgorithmOptionButton[0]->SetColor(unselectedButtonColor);
    statusScript->shadowAlgorithmOptionText[0]->SetColor(unselectedTextColor);
    statusScript->shadowAlgorithmOptionButton[1]->SetColor(unselectedButtonColor);
    statusScript->shadowAlgorithmOptionText[1]->SetColor(unselectedTextColor);
    statusScript->shadowAlgorithmOptionButton[2]->SetColor(selectedButtonColor);
    statusScript->shadowAlgorithmOptionText[2]->SetColor(selectedTextColor);

    statusScript->girlRenderer->SetMaterial(statusScript->girlMaterial[2]);
    statusScript->tileRenderer->SetMaterial(statusScript->tileMaterial[2]);
    Light::GetMainLight()->SetShadowAlgorithmVSSM();
}

void StatusScript::OnPressViewOptionButton0(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->viewOptionButton[0]->SetColor(selectedButtonColor);
    statusScript->viewOptionButton[1]->SetColor(unselectedButtonColor);
    statusScript->viewOptionText[0]->SetColor(selectedTextColor);
    statusScript->viewOptionText[1]->SetColor(unselectedTextColor);

    Transform *transform = Camera::GetMainCamera()->GetGameObject()->GetTransform();
    transform->SetPosition(sideViewPosition);
    transform->SetLocalRotation(sideViewRotation);
}

void StatusScript::OnPressViewOptionButton1(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->viewOptionButton[0]->SetColor(unselectedButtonColor);
    statusScript->viewOptionButton[1]->SetColor(selectedButtonColor);
    statusScript->viewOptionText[0]->SetColor(unselectedTextColor);
    statusScript->viewOptionText[1]->SetColor(selectedTextColor);

    Transform *transform = Camera::GetMainCamera()->GetGameObject()->GetTransform();
    transform->SetPosition(topViewPosition);
    transform->SetLocalRotation(topViewRotation);
}