#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "Component.h"

class Camera : public Component {
private:
    static Camera *mainCamera;

public:
    static Camera *GetMainCamera();
    static void SetMainCamera(Camera *camera);

private:
    glm::mat4 normalization;

public:
    Camera(const glm::mat4 &normalization);
    virtual ~Camera();

    void Render();
};

#endif