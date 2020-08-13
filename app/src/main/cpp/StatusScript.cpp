#include "StatusScript.h"
#include "Text.h"
#include "Time.h"
#include "Button.h"
#include "Light.h"

using namespace std;
using namespace glm;

vec4 StatusScript::unselectedButtonColor = vec4(1.0f);
vec4 StatusScript::selectedButtonColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);
vec4 StatusScript::unselectedTextColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
vec4 StatusScript::selectedTextColor = vec4(1.0f);

StatusScript::StatusScript(Text *fpsText, Text *shadowMapSizeText, Button *shadowMapSizeButton0,
                           Button *shadowMapSizeButton1, Text *shadowMapSizeOptionText0, Text *shadowMapSizeOptionText1)
                           : fpsText(fpsText), shadowMapSizeText(shadowMapSizeText) {
    shadowMapSizeButton[0] = shadowMapSizeButton0;
    shadowMapSizeButton[1] = shadowMapSizeButton1;
    shadowMapSizeOptionText[0] = shadowMapSizeOptionText0;
    shadowMapSizeOptionText[1] = shadowMapSizeOptionText1;

    shadowMapSizeButton[0]->RegisterOnPressEvent(this, OnPressShadowMapSizeButton0);
    shadowMapSizeButton[1]->RegisterOnPressEvent(this, OnPressShadowMapSizeButton1);

    shadowMapSizeButton[0]->SetColor(selectedButtonColor);
    shadowMapSizeButton[1]->SetColor(unselectedButtonColor);
    shadowMapSizeOptionText[0]->SetColor(selectedTextColor);
    shadowMapSizeOptionText[1]->SetColor(unselectedTextColor);

    Light::GetMainLight()->SetShadowMapSizeSmall();
}

StatusScript::~StatusScript() {

}

void StatusScript::OnUpdate() {
    int fps = Time::Fps();
    string text = "# FPS: ";
    text += to_string(fps);
    fpsText->SetString(text);
}

void StatusScript::OnPressShadowMapSizeButton0(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowMapSizeButton[0]->SetColor(selectedButtonColor);
    statusScript->shadowMapSizeButton[1]->SetColor(unselectedButtonColor);
    statusScript->shadowMapSizeOptionText[0]->SetColor(selectedTextColor);
    statusScript->shadowMapSizeOptionText[1]->SetColor(unselectedTextColor);

    Light::GetMainLight()->SetShadowMapSizeSmall();
}

void StatusScript::OnPressShadowMapSizeButton1(void *ptr) {
    StatusScript *statusScript = (StatusScript *)ptr;
    statusScript->shadowMapSizeButton[0]->SetColor(unselectedButtonColor);
    statusScript->shadowMapSizeButton[1]->SetColor(selectedButtonColor);
    statusScript->shadowMapSizeOptionText[0]->SetColor(unselectedTextColor);
    statusScript->shadowMapSizeOptionText[1]->SetColor(selectedTextColor);

    Light::GetMainLight()->SetShadowMapSizeBig();
}