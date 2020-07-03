#include <stb/stb_image.h>
#include "NDKHelper.h"

#include "Texture.h"

using namespace std;

Texture::Texture(const string &path) {
    std::vector<uint8_t> buf;
    if (!ndk_helper::JNIHelper::GetInstance()->ReadFile(path.c_str(), &buf)) {
        LOGI("[ %s ] failed reading file: %s", __FUNCTION__, path.c_str());
        throw exception();
    }

    int width, height, channel;
    uint8_t *image = stbi_load_from_memory(buf.data(), buf.size(), &width, &height, &channel, 0);

    GLenum format;
    switch (channel) {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}