#include <EGL/egl.h>

#include "Slider.h"
#include "GUI.h"
#include "Shader.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "Texture.h"

using namespace std;
using namespace glm;

extern EGLint width;
extern EGLint height;

std::unordered_set<Slider *> Slider::sliders;

void Slider::CheckIfValueChanged(const glm::vec2 &pointer) {
    for (Slider *slider : sliders) {
        Transform *transform = slider->GetGameObject()->GetTransform();
        vec3 pos = transform->GetLocalPosition();
        vec3 scale = transform->GetLocalScale();
        float left = pos.x;
        float right = pos.x + slider->size.x * scale.x;
        float bottom = pos.y - slider->size.y * scale.y * 0.5f;
        float top = pos.y + slider->size.y * scale.y * 1.5f;
        if (pointer.x >= left && pointer.x <= right && height - pointer.y >= bottom && height - pointer.y <= top) {
            slider->value = (pointer.x - left) / (right - left);
        }
    }
}

Slider::Slider(Texture *texture, Texture *handleTexture, const glm::vec2 &size, float thickness,
               const glm::vec4 &color, const glm::vec4 &handleColor, int renderOrder)
               : GUIComponent(texture, size, color, renderOrder), handleTexture(handleTexture), thickness(thickness), handleColor(handleColor) {
    value = 0.0f;

    sliders.insert(this);
}

Slider::~Slider() {
    sliders.erase(this);
}

void Slider::Render() {
    if (!texture) {
        return;
    }

    bool blend_enabled = glIsEnabled(GL_BLEND);
    GLint blend_src, blend_dst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Transform *transform = GetGameObject()->GetTransform();
    vec3 pos = transform->GetLocalPosition();
    vec3 scale = transform->GetLocalScale();

    // draw slider
    GUI::uiMaterial->SetVector("_COLOR", color);
    GUI::uiMaterial->SetMatrix("_NORM", ortho(0.0f, (float)width, 0.0f, (float)height));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(GUI::vao);

    float xpos = pos.x;
    float ypos = pos.y + (size.y - thickness) * scale.y * 0.5f;

    float w = size.x * scale.x;
    float h = thickness * scale.y;

    float slider_vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f }
    };
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glBindBuffer(GL_ARRAY_BUFFER, GUI::vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(slider_vertices), slider_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw handle
    GUI::uiMaterial->SetVector("_COLOR", handleColor);

    xpos = pos.x + size.x * scale.x * value - size.y * scale.y * 0.5f;
    ypos = pos.y;

    w = size.y * scale.y;
    h = w;

    float handle_vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f }
    };
    glBindTexture(GL_TEXTURE_2D, handleTexture->id);

    glBindBuffer(GL_ARRAY_BUFFER, GUI::vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(handle_vertices), handle_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (blend_enabled) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
    glBlendFunc(blend_src, blend_dst);
}