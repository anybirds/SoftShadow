#include "NDKHelper.h"

#include "Shader.h"

#define MAX_SHADER_INFO_LOG_LENGTH 512

using namespace std;

char infoLog[MAX_SHADER_INFO_LOG_LENGTH];

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

    buf.push_back('\0'); // put null character at the end
    const char *src = (const char *)buf.data();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderInfoLog(id, MAX_SHADER_INFO_LOG_LENGTH, nullptr, infoLog);
        LOGI("[ %s ] failed compiling shader: %s\n%s", __FUNCTION__, path.c_str(), infoLog);
        glDeleteShader(id);
        throw exception();
    }
}

Shader::~Shader() {
    glDeleteShader(id);
}
