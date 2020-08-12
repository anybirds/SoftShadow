#include <EGL/egl.h>

#include "GUIComponent.h"
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

set<pair<int, GUIComponent *>> GUIComponent::guiComponents;

GUIComponent::GUIComponent(const glm::vec4 &color, int renderOrder) : color(color), renderOrder(renderOrder) {
    guiComponents.insert(make_pair(renderOrder, this));
}

GUIComponent::GUIComponent(Texture *texture, const glm::vec2 &size, const glm::vec4 &color, int renderOrder) : texture(texture), size(size), color(color), renderOrder(renderOrder) {
    guiComponents.insert(make_pair(renderOrder, this));
}

GUIComponent::~GUIComponent() {
    guiComponents.erase(make_pair(renderOrder, this));
}

void GUIComponent::Render() {
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
    GUI::uiMaterial->SetVector("_COLOR", color);
    GUI::uiMaterial->SetMatrix("_NORM", ortho(0.0f, (float)width, 0.0f, (float)height));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(GUI::vao);

    float xpos = pos.x;
    float ypos = pos.y;

    float w = size.x * scale.x;
    float h = size.y * scale.y;

    float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f }
    };
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glBindBuffer(GL_ARRAY_BUFFER, GUI::vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (blend_enabled) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
    glBlendFunc(blend_src, blend_dst);
}