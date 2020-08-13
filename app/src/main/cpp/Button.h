#ifndef BUTTON_H
#define BUTTON_H

#include <unordered_set>
#include <functional>
#include <glm/glm.hpp>

#include "GUIComponent.h"

class Texture;

class Button : public GUIComponent {
private:
    static std::unordered_set<Button *> buttons;
public:
    static void CheckIfPressed(const glm::vec2 &pointer);

private:
    void *onPressListener;
    std::function<void(void *)> onPress;

public:
    Button(Texture *texture, const glm::vec2 &size, const glm::vec4 &color = glm::vec4(1.0f), int renderOrder = 0);
    virtual ~Button();

    void RegisterOnPressEvent(void *onPressListener, std::function<void(void *)> onPress) { this->onPressListener = onPressListener, this->onPress = onPress; }

};

#endif