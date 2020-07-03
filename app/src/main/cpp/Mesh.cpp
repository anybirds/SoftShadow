#include "NDKHelper.h"

#include "Mesh.h"
#include "Model.h"

using namespace std;

Mesh::Mesh(Model *model, int index) : model(model), index(index) {
    const aiScene *scene = model->importer->GetScene();
    if (!scene->HasMeshes() || scene->mNumMeshes <= index) {
        LOGI("[ %s ] model: %s does not contain any mesh", __FUNCTION__, model->path.c_str());
        throw exception();
    }

    aiMesh *aimesh = scene->mMeshes[index];
    unsigned base;

    unsigned acnt = 3;
    unsigned *attrib = new unsigned[acnt]();
    if (aimesh->HasPositions()) {
        attrib[0] = 3;
    }
    if (aimesh->HasNormals()) {
        attrib[1] = 3;
    }
    if (aimesh->HasTextureCoords(0)) {
        attrib[2] = 2;
    }
    unsigned asize = 0; // number of floats for each vertex
    for (unsigned i = 0; i < acnt; i++) {
        asize += attrib[i];
    }

    vcnt = aimesh->mNumVertices;
    float *vert = new float[vcnt * asize];
    base = 0;
    for (unsigned i = 0; i < aimesh->mNumVertices; i++) {
        if (aimesh->HasPositions()) {
            vert[base] = aimesh->mVertices[i].x;
            vert[base + 1] = aimesh->mVertices[i].y;
            vert[base + 2] = aimesh->mVertices[i].z;
            base += 3;
        }
        if (aimesh->HasNormals()) {
            vert[base] = aimesh->mNormals[i].x;
            vert[base + 1] = aimesh->mNormals[i].y;
            vert[base + 2] = aimesh->mNormals[i].z;
            base += 3;
        }
        if (aimesh->HasTextureCoords(0)) { // texture coordinate set 0
            vert[base] = aimesh->mTextureCoords[0][i].x;
            vert[base + 1] = aimesh->mTextureCoords[0][i].y;
            base += 2;
        }
    }

    icnt = aimesh->mNumFaces * 3;
    unsigned *idx = new unsigned[icnt];
    base = 0;
    for (unsigned i = 0; i < aimesh->mNumFaces; i++) {
        idx[base] = aimesh->mFaces[i].mIndices[0];
        idx[base + 1] = aimesh->mFaces[i].mIndices[1];
        idx[base + 2] = aimesh->mFaces[i].mIndices[2];
        base += 3;
    }

    // generate
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // data
    unsigned stride = 0;
    for (unsigned i = 0; i < acnt; i++) {
        stride += attrib[i];
    }
    stride *= sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, vcnt * stride, vert, GL_STATIC_DRAW);

    // attribute
    float *offset = nullptr;
    for (unsigned i = 0; i < acnt; i++) {
        if (attrib[i]) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, attrib[i], GL_FLOAT, GL_FALSE, stride, (void *)offset);
            offset += attrib[i];
        }
    }

    if (icnt > 0) {
        // indexed wireframe
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, icnt * sizeof(unsigned), idx, GL_STATIC_DRAW);
    }

    // restore
    glBindVertexArray(0);
    delete[] vert;
    delete[] attrib;
    delete[] idx;

    LOGI("[ %s ] mesh created with %d vertices from model: %s", __FUNCTION__, vcnt, model->path.c_str());
}

Mesh::~Mesh() {
    if (icnt > 0) {
        // indexed wireframe
        glDeleteBuffers(1, &ebo);
    }
    // delete
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}