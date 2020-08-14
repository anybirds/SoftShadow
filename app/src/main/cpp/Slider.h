#ifndef SLIDER_H
#define SLIDER_H

#include <unordered_set>
#include "GUIComponent.h"

class Slider : public GUIComponent {
private:
    static std::unordered_set<Slider *> sliders;

public:
    static void CheckIfValueChanged(const glm::vec2 &pointer);

private:
    float thickness;
    Texture *handleTexture;
    glm::vec4 handleColor;

    float value;

public:
    Slider(Texture *texture, Texture *handleTexture, const glm::vec2 &size, float thickness, const glm::vec4 &color = glm::vec4(1.0f), const glm::vec4 &handleColor = glm::vec4(1.0f), int renderOrder = 0);
    virtual ~Slider();

    virtual void Render() override;

    float GetValue() { return value; }
};

#endif