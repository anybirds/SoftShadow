#include "GameObject.h"
#include "Transform.h"

using namespace glm;

GameObject::GameObject() {
    transform = new Transform(vec3(0.0f), quat(1.0f, 0.0f, 0.0f, 0.0f), vec3(1.0f), nullptr);
    AddComponent<Transform>(transform);
}

GameObject::GameObject(const glm::vec3 &localPosition, const glm::quat &localRotation,
                       const glm::vec3 &localScale, Transform *parent) {
    transform = new Transform(localPosition, localRotation, localScale, parent);
    AddComponent<Transform>(transform);
}

GameObject::~GameObject() {
    delete transform;
}