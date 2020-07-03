#include "NDKHelper.h"

#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"

using namespace std;

unordered_set<Renderer *> Renderer::renderers;

Renderer::Renderer(GameObject *gameObject, Material *material, Mesh *mesh) : Component(gameObject), material(material), mesh(mesh) {
    renderers.insert(this);
}

Renderer::~Renderer() {
    renderers.erase(this);
}