#ifndef PANEL_H
#define PANEL_H

#include <glm/glm.hpp>

#include "GUIComponent.h"

class Panel : public GUIComponent {
public:
    Panel(Texture *texture, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f), int renderOrder = 0);
    virtual ~Panel();
};

#endif