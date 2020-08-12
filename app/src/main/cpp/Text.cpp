#include <EGL/egl.h>

#include "Font.h"
#include "GameObject.h"
#include "Material.h"
#include "Shader.h"
#include "Text.h"
#include "Transform.h"
#include "GUI.h"

#include "NDKHelper.h"

using namespace std;
using namespace glm;

extern EGLint width;
extern EGLint height;

Text::Text(const string &text, Font *font, const vec4 &color, int renderOrder) : GUIComponent(color, renderOrder), text(text), font(font) {

}

Text::~Text() {

}

void Text::Render() {
    bool blend_enabled = glIsEnabled(GL_BLEND);
    GLint blend_src, blend_dst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Transform *transform = GetGameObject()->GetTransform();
    vec3 pos = transform->GetLocalPosition();
    vec3 scale = transform->GetLocalScale();
    GUI::textMaterial->SetVector("_COLOR", color);
    GUI::textMaterial->SetMatrix("_NORM", ortho(0.0f, (float)width, 0.0f, (float)height));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(GUI::vao);

    for (int i=0; i<text.size(); i++) {
        Font::Character character = font->characters[text[i]];

        float xpos = pos.x + character.bearing.x * scale.x;
        float ypos = pos.y - (character.size.y - character.bearing.y) * scale.y;

        float w = character.size.x * scale.x;
        float h = character.size.y * scale.y;
        // update VBO for each character
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, character.textureId);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, GUI::vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        pos.x += (character.advance >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    if (blend_enabled) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
    glBlendFunc(blend_src, blend_dst);
}