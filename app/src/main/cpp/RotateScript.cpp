#include "GameObject.h"
#include "Time.h"
#include "Transform.h"

#include "RotateScript.h"

using namespace glm;

void RotateScript::OnUpdate() {
    float dt = Time::DeltaTime();
    float rate = 90.0f;
    Transform *transform = GetGameObject()->GetTransform();
    transform->RotateAround(vec3(0.0f, 0.0f, 1.0f), rate * dt);
}