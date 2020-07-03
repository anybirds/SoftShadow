#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
private:
    std::string path;
    Assimp::Importer *importer;

public:
    Model(const std::string &path);
    virtual ~Model();

    friend class Mesh;
};

#endif