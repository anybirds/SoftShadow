#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_set>

#include "Component.h"

class Material;
class Mesh;

class Renderer : public Component {
private:
    static std::unordered_set<Renderer *> renderers;

private:
    Material *material;
    Mesh *mesh;

public:
    Renderer(GameObject *gameObject, Material *material, Mesh *mesh);
    virtual ~Renderer();

    Material *GetMaterial() const { return material; }
    void SetMaterial(Material *material) { this->material = material; }
    Mesh *GetMesh() const { return mesh; }
    void SetMesh(Mesh *mesh) { this->mesh = mesh; }

    friend class Camera;
};

#endif