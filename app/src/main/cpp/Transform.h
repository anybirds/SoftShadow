#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Component.h"

class Transform : public Component {
private:
    glm::vec3 localPosition;
    glm::quat localRotation;
    glm::vec3 localScale;
    glm::vec3 localEulerAngles;

    Transform *parent;
    std::vector<Transform *> children;

    mutable bool updated;
    mutable glm::mat4 localToWorldMatrix;

    void PropagateUpdate();

public:
    Transform(GameObject *gameObject, const glm::vec3 &localPosition, const glm::quat &localRotation, const glm::vec3 &localScale, Transform *parent = nullptr);
    virtual ~Transform();

    const glm::vec3 &GetLocalPosition() const { return localPosition; }
    const glm::quat &GetLocalRotation() const { return localRotation; }
    const glm::vec3 &GetLocalScale() const { return localScale; }
    const glm::vec3 &GetLocalEulerAngles() const { return localEulerAngles; }
    Transform *GetParent() const { return parent; }

    glm::mat4 GetLocalToWorldMatrix() const;
    glm::mat4 GetWorldToLocalMatrix() const;
    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;

    void SetLocalPosition(const glm::vec3 &localPosition);
    void SetLocalRotation(const glm::quat &localRotation);
    void SetLocalScale(const glm::vec3 &localScale);
    void SetLocalEulerAngles(const glm::vec3 &localEulerAngles);
    void SetPosition(const glm::vec3 &position);
    // void SetRotation(const glm::quat &rotation);
    void SetScale(const glm::vec3 &scale);

    void Rotate(const glm::vec3 &eulerAngles);
    void RotateAround(const glm::vec3 &axis, float angle);
};

#endif