#include "NDKHelper.h"

#include "Shader.h"

using namespace std;

Shader::Shader(const string &path, GLenum type) : path(path), type(type) {
    id = glCreateShader(type);

    if (!id) {
        LOGI("[ %s ] failed creating a new shader: %s", __FUNCTION__, path.c_str());
        throw exception();
    }

    std::vector<uint8_t> buf;
    if (!ndk_helper::JNIHelper::GetInstance()->ReadFile(path.c_str(), &buf)) {
        LOGI("[ %s ] failed reading file: %s", __FUNCTION__, path.c_str());
        throw exception();
    }

    const char *src = (const char *)buf.data();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        LOGI("[ %s ] failed compiling shader: %s", __FUNCTION__, path.c_str());
        glDeleteShader(id);
        throw exception();
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}
