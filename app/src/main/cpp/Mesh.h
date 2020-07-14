#ifndef MESH_H
#define MESH_H

#include <GLES3/gl3.h>

class Model;

class Mesh {
private:
    Model *model;
    int index;

    unsigned vcnt;
    unsigned icnt;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

public:
    Mesh(Model *model, int index = 0);
    virtual ~Mesh();

    friend class Camera;
    friend class Light;
};

#endif