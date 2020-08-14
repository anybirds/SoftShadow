#ifndef STATUS_SCRIPT_H
#define STATUS_SCRIPT_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Script.h"

class Text;
class Button;
class Shader;
class Material;
class Renderer;
class Slider;

class StatusScript : public Script {
private:
    static glm::vec4 unselectedButtonColor;
    static glm::vec4 selectedButtonColor;
    static glm::vec4 unselectedTextColor;
    static glm::vec4 selectedTextColor;

    static glm::vec3 topViewPosition;
    static glm::quat topViewRotation;
    static glm::vec3 sideViewPosition;
    static glm::quat sideViewRotation;

    static void OnPressShadowMapSizeOptionButton0(void *ptr);
    static void OnPressShadowMapSizeOptionButton1(void *ptr);

    static void OnPressShadowAlgorithmOptionButton0(void *ptr);
    static void OnPressShadowAlgorithmOptionButton1(void *ptr);
    static void OnPressShadowAlgorithmOptionButton2(void *ptr);

    static void OnPressViewOptionButton0(void *ptr);
    static void OnPressViewOptionButton1(void *ptr);

private:
    Text *fpsText;
    Button *shadowMapSizeOptionButton[2];
    Text *shadowMapSizeOptionText[2];
    Button *shadowAlgorithmOptionButton[3];
    Text *shadowAlgorithmOptionText[3];
    Slider *areaSlider;
    Button *viewOptionButton[2];
    Text *viewOptionText[2];

    Renderer *girlRenderer;
    Renderer *tileRenderer;
    Material *girlMaterial[3];
    Material *tileMaterial[3];

public:
    StatusScript(Text *fpsText,
            Button *shadowMapSizeOptionButton0, Button *shadowMapSizeOptionButton1, Text *shadowMapSizeOptionText0, Text *shadowMapSizeOptionText1,
            Button *shadowAlgorithmOptionButton0, Button *shadowAlgorithmOptionButton1, Button *shadowAlgorithmOptionButton2, Text *shadowAlgorithmOptionText0, Text *shadowAlgorithmOptionText1, Text *shadowAlgorithmOptionText2,
            Renderer *girlRenderer, Renderer *tileRenderer, Material *girlMaterial0, Material *girlMaterial1, Material *girlMaterial2, Material *tileMaterial0, Material *tileMaterial1, Material *tileMaterial2,
            Slider *areaSlider,
            Button *viewOptionButton0, Button *viewOptionButton1, Text *viewOptionText0, Text *viewOptionText1);
    virtual ~StatusScript();

    virtual void OnUpdate() override;
};

#endif