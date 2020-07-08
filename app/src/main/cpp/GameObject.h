#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <unordered_map>
#include <typeinfo>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Component.h"

class Transform;

class GameObject {
private:
    Transform *transform;
    std::unordered_map<size_t, Component *> components;

public:
    GameObject();
    GameObject(const glm::vec3 &localPosition, const glm::quat &localRotation, const glm::vec3 &localScale, Transform *parent = nullptr);
    virtual ~GameObject();

    Transform *GetTransform() const { return transform; }
    void SetTransform(Transform *transform) { this->transform = transform; }

    template <typename T>
    T *GetComponent();
    template <typename T>
    bool AddComponent(Component *component);
    template <typename T>
    bool RemoveComponent(Component *component);

    friend class Component;
};

template <typename T>
T *GameObject::GetComponent() {
    size_t key = typeid(T).hash_code();
    auto it = components.find(key);
    if (it == components.end()) {
        return nullptr;
    }
    return dynamic_cast<T*>(it->second);
}

template <typename T>
bool GameObject::AddComponent(Component *component) {
    size_t key = typeid(T).hash_code();
    auto it = components.find(key);
    if (it != components.end()) {
        return false;
    }
    components.insert({key, component});
    component->SetGameObject(this);
    return true;
}

template <typename T>
bool GameObject::RemoveComponent(Component *component) {
    size_t key = typeid(T).hash_code();
    auto it = components.find(key);
    if (it == components.end()) {
        return false;
    }
    components.erase(it);
    component->SetGameObject(nullptr);
    return true;
}

#endif