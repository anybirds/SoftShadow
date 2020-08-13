#ifndef STATUS_SCRIPT_H
#define STATUS_SCRIPT_H

#include <glm/glm.hpp>

#include "Script.h"

class Text;
class Button;

class StatusScript : public Script {
private:
    static glm::vec4 unselectedButtonColor;
    static glm::vec4 selectedButtonColor;
    static glm::vec4 unselectedTextColor;
    static glm::vec4 selectedTextColor;

    static void OnPressShadowMapSizeButton0(void *ptr);
    static void OnPressShadowMapSizeButton1(void *ptr);

private:
    Text *fpsText;
    Text *shadowMapSizeText;
    Button *shadowMapSizeButton[2];
    Text *shadowMapSizeOptionText[2];
public:
    StatusScript(Text *fpsText, Text *shadowMapSizeText, Button *shadowMapSizeButton0, Button *shadowMapSizeButton1, Text *shadowMapSizeOptionText0, Text *shadowMapSizeOptionText1);
    virtual ~StatusScript();

    virtual void OnUpdate() override;
};

#endif