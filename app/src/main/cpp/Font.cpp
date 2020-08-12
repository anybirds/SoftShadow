#include <vector>
#include <GLES3/gl3.h>

#include "Font.h"
#include "GUI.h"

#include "NDKHelper.h"

using namespace std;

Font::Font(const string &path) {
    std::vector<uint8_t> buf;
    if (!ndk_helper::JNIHelper::GetInstance()->ReadFile(path.c_str(), &buf)) {
        LOGI("[ %s ] failed reading file: %s", __FUNCTION__, path.c_str());
        throw exception();
    }

    FT_Face face;
    if (FT_New_Memory_Face(GUI::ft, &buf[0], buf.size(), 0, &face)) {
        LOGI("[ %s ] failed loading font: %s", __FUNCTION__, path.c_str());
        throw exception();
    }

    FT_Set_Pixel_Sizes(face, 0, 36);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_R8,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (unsigned int)face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
}

Font::~Font() {

}
