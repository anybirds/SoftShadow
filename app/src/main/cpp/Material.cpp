#include "NDKHelper.h"

#include "Material.h"
#include "Shader.h"
#include "Texture.h"

using namespace std;
using namespace glm;

Material::Material(Shader *vertexShader, Shader *fragmentShader) : vertexShader(vertexShader), fragmentShader(fragmentShader) {
    if (vertexShader->type != GL_VERTEX_SHADER || fragmentShader->type != GL_FRAGMENT_SHADER) {
        LOGI("[ %s ] shader type mismatch", __FUNCTION__);
        throw exception();
    }

    // attach shaders and link
    program = glCreateProgram();
    glAttachShader(program, vertexShader->id);
    glAttachShader(program, fragmentShader->id);
    glLinkProgram(program);
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
    GLint location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

void Material::SetIntegerArray(const char *name, const int *value, int length) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1iv(location, length, value);
}

void Material::SetFloat(const char *name, float value) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

void Material::SetFloatArray(const char *name, const float *value, int length) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1fv(location, length, value);
}

void Material::SetVector(const char *name, const vec4 &value) {
    GLint location = glGetUniformLocation(program, name);
    glUniform4fv(location, 1, (const GLfloat *)&value);
}

void Material::SetVectorArray(const char *name, const vec4 *value, int length) {
    GLint location = glGetUniformLocation(program, name);
    glUniform4fv(location, length, (const GLfloat *)value);
}

void Material::SetMatrix(const char *name, const mat4 &value) {
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *)&value);
}

void Material::SetMatrixArray(const char *name, const mat4 *value, int length) {
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, length, GL_FALSE, (const GLfloat *)value);
}

void Material::UseTextures() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainTexture->id);

    // sampler uniform values
    GLint location = glGetUniformLocation(program, "_MAIN_TEX");
    glUniform1i(location, 0);
}