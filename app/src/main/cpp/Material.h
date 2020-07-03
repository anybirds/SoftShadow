#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include <GLES3/gl3.h>

#include <glm/glm.hpp>

class Shader;
class Texture;

class Material {
private:
    Shader *vertexShader;
    Shader *fragmentShader;
    Texture *mainTexture;

    GLuint program;

public:
    Material(Shader *vertexShader, Shader *fragmentShader);
    virtual ~Material();

    Texture *GetMainTexture() const { return mainTexture; }
    void SetMainTexture(Texture *mainTexture) { this->mainTexture = mainTexture; }

    int GetInteger(const char *name) const;
    std::vector<int> GetIntegerArray(const char *name) const;
    float GetFloat(const char *name) const;
    std::vector<float> GetFloatArray(const char *name) const;
    glm::vec4 GetVector(const char *name) const;
    std::vector<glm::vec4> GetVectorArray(const char *name) const;
    glm::mat4 GetMatrix(const char *name) const;
    std::vector<glm::mat4> GetMatrixArray(const char *name) const;

    void SetInteger(const char *name, int value);
    void SetIntegerArray(const char *name, const int *value, int length);
    void SetFloat(const char *name, float value);
    void SetFloatArray(const char *name, const float *value, int length);
    void SetVector(const char *name, const glm::vec4 &value);
    void SetVectorArray(const char *name, const glm::vec4 *value, int length);
    void SetMatrix(const char *name, const glm::mat4 &value);
    void SetMatrixArray(const char *name, const glm::mat4 *value, int length);

    void UseTextures();
};

#endif