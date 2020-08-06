#include "NDKHelper.h"

#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"

#define MAX_PROGRAM_INFO_LOG_LENGTH 512

using namespace std;
using namespace glm;

char programInfoLog[MAX_PROGRAM_INFO_LOG_LENGTH];

Material::Material(Shader *vertexShader, Shader *fragmentShader) : vertexShader(vertexShader), fragmentShader(fragmentShader) {
    if (vertexShader->type != GL_VERTEX_SHADER || fragmentShader->type != GL_FRAGMENT_SHADER) {
        LOGI("[ %s ] shader type mismatch", __FUNCTION__);
        throw exception();
    }
    GLenum err;
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror before mat : %d", err);
    }
    // attach shaders and link
    program = glCreateProgram();
    glAttachShader(program, vertexShader->id);
    glAttachShader(program, fragmentShader->id);
    glLinkProgram(program);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror after link : %d", err);
    }

    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glGetProgramInfoLog(program, MAX_PROGRAM_INFO_LOG_LENGTH, nullptr, programInfoLog);
        LOGI("[ %s ] failed linking program\n%s", __FUNCTION__, programInfoLog);
        glDeleteProgram(program);
        throw exception();
    }
    // sampler uniform values
    glUseProgram(program);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGI("glerror after link : %d", err);
    }
    GLint location = glGetUniformLocation(program, "_MAIN_TEX");
    glUniform1i(location, 0);
    location = glGetUniformLocation(program, "_SHADOW_MAP");
    glUniform1i(location, 1);
    location = glGetUniformLocation(program, "_SHADOW_MAP_PCF");
    glUniform1i(location, 2);
    location = glGetUniformLocation(program, "_HSM");
    glUniform1i(location, 3);
    location = glGetUniformLocation(program, "_VSM");
    glUniform1i(location, 4);
}

Material::~Material() {
    glDeleteProgram(program);
}


int Material::GetInteger(const char *name) const {
    int ret = 0;
    GLint location = glGetUniformLocation(program, name);
    if (location != -1) {
        glGetUniformiv(program, location, &ret);
    }
    return ret;
}
vector<int> Material::GetIntegerArray(const char *name) const {
    vector<int> ret;
    GLint location;
    string str(name);
    for (int i=0; ; i++) {
        string elem = str + '[' + to_string(i) + ']';
        location = glGetUniformLocation(program, elem.c_str());
        if (location == -1) {
            break;
        }
        int value;
        glGetUniformiv(program, location, &value);
        ret.push_back(value);
    }
    return ret;
}
float Material::GetFloat(const char *name) const {
    float ret = 0.0f;
    GLint location = glGetUniformLocation(program, name);
    if (location != -1) {
        glGetUniformfv(program, location, &ret);
    }
    return ret;
}
vector<float> Material::GetFloatArray(const char *name) const {
    vector<float> ret;
    GLint location;
    string str(name);
    for (int i=0; ; i++) {
        string elem = str + '[' + to_string(i) + ']';
        location = glGetUniformLocation(program, elem.c_str());
        if (location == -1) {
            break;
        }
        float value;
        glGetUniformfv(program, location, &value);
        ret.push_back(value);
    }
    return ret;
}
vec4 Material::GetVector(const char *name) const {
    vec4 ret;
    GLint location = glGetUniformLocation(program, name);
    if (location != -1) {
        glGetUniformfv(program, location, (GLfloat *)&ret);
    }
    return ret;
}
vector<vec4> Material::GetVectorArray(const char *name) const {
    vector<vec4> ret;
    GLint location;
    string str(name);
    for (int i=0; ; i++) {
        string elem = str + '[' + to_string(i) + ']';
        location = glGetUniformLocation(program, elem.c_str());
        if (location == -1) {
            break;
        }
        vec4 value;
        glGetUniformfv(program, location, (GLfloat *)&value);
        ret.push_back(value);
    }
    return ret;
}
mat4 Material::GetMatrix(const char *name) const {
    mat4 ret;
    GLint location = glGetUniformLocation(program, name);
    if (location != -1) {
        glGetUniformfv(program, location, (GLfloat *)&ret);
    }
    return ret;
}

vector<mat4> Material::GetMatrixArray(const char *name) const {
    vector<mat4> ret;
    GLint location;
    string str(name);
    for (int i=0; ; i++) {
        string elem = str + '[' + to_string(i) + ']';
        location = glGetUniformLocation(program, elem.c_str());
        if (location == -1) {
            break;
        }
        mat4 value;
        glGetUniformfv(program, location, (GLfloat *)&value);
        ret.push_back(value);
    }
    return ret;
}

void Material::SetInteger(const char *name, int value) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

void Material::SetIntegerArray(const char *name, const int *value, int length) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform1iv(location, length, value);
}

void Material::SetFloat(const char *name, float value) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

void Material::SetFloatArray(const char *name, const float *value, int length) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform1fv(location, length, value);
}

void Material::SetVector(const char *name, const vec2 &value) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform2fv(location, 1, (const GLfloat *)&value);
}

void Material::SetVector(const char *name, const vec3 &value) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform3fv(location, 1, (const GLfloat *)&value);
}

void Material::SetVector(const char *name, const vec4 &value) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform4fv(location, 1, (const GLfloat *)&value);
}

void Material::SetVectorArray(const char *name, const vec2 *value, int length) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform2fv(location, length, (const GLfloat *)value);
}

void Material::SetVectorArray(const char *name, const vec3 *value, int length) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform3fv(location, length, (const GLfloat *)value);
}

void Material::SetVectorArray(const char *name, const vec4 *value, int length) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniform4fv(location, length, (const GLfloat *)value);
}

void Material::SetMatrix(const char *name, const mat4 &value) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&value);
}

void Material::SetMatrixArray(const char *name, const mat4 *value, int length) {
    glUseProgram(program);
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, length, GL_FALSE, (const GLfloat *)value);
}

void Material::UseTextures() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainTexture->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Light::GetMainLight()->shadowMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, Light::GetMainLight()->shadowMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, Light::GetMainLight()->hsm);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, Light::GetMainLight()->vsm);
}