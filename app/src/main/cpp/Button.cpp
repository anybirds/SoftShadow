#include <EGL/egl.h>

#include "Button.h"
#include "Shader.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"
#include "GUI.h"

#include "NDKHelper.h"

using namespace glm;
using namespace std;

extern EGLint width;
extern EGLint height;

unordered_set<Button *> Button::buttons;

void Button::CheckIfPressed(const vec2 &pointer) {
    LOGI("%f %f", pointer.x, height - pointer.y);
    for (Button *button : buttons) {
        Transform *transform = button->GetGameObject()->GetTransform();
        vec3 pos = transform->GetLocalPosition();
        vec3 scale = transform->GetLocalScale();
        float left = pos.x;
        float right = pos.x + button->size.x;
        float bottom = pos.y;
        float top = pos.y + button->size.y;
        if (pointer.x >= left && pointer.x <= right && height - pointer.y >= bottom && height - pointer.y <= top) {
            LOGI("pressed");
            if (button->onPressListener && button->onPress) {
                button->onPress(button->onPressListener);
            }
        }
    }
}

Button::Button(Texture *texture, const vec2 &size, const vec4 &color, int renderOrder) : GUIComponent(texture, size, color, renderOrder) {
    buttons.insert(this);
}

Button::~Button() {
    buttons.erase(this);
}