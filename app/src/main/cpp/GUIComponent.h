#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <set>
#include <glm/glm.hpp>
#include "Component.h"

class Texture;

class GUIComponent : public Component {
private:
    static std::set<std::pair<int, GUIComponent *>> guiComponents;

protected:
    Texture *texture;
    glm::vec2 size;
    glm::vec4 color;
    int renderOrder;

public:
    GUIComponent(const glm::vec4 &color = glm::vec4(1.0f), int renderOrder = 0);
    GUIComponent(Texture *texture, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f), int renderOrder = 0);
    virtual ~GUIComponent();

    virtual void Render();

    void SetColor(const glm::vec4 &color) { this->color = color; }

    friend class Camera;
};
#endif