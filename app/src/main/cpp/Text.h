#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <unordered_set>
#include <glm/glm.hpp>

#include "GUIComponent.h"

class Font;

class Text : public GUIComponent {
private:
    std::string text;
    Font *font;

public:
    Text(const std::string &text, Font *font, const glm::vec4 &color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), int renderOrder = 0);
    virtual ~Text();

    virtual void Render() override;

    const std::string &GetString() { return text; }
    void SetString(const std::string &text) { this->text = text; }
};

#endif