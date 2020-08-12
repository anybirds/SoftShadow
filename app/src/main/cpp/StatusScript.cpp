#include "StatusScript.h"
#include "Text.h"
#include "Time.h"

using namespace std;

StatusScript::StatusScript(Text *fpsText) : fpsText(fpsText) {

}

StatusScript::~StatusScript() {

}

void StatusScript::OnUpdate() {
    int fps = Time::Fps();
    string text = "# FPS: ";
    text += to_string(fps);
    fpsText->SetString(text);
}