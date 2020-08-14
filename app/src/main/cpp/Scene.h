#ifndef SCENE_H
#define SCENE_H

class Camera;
class GameObject;
class Light;
class Material;
class Mesh;
class Model;
class Renderer;
class Shader;
class Texture;
class RotateScript;
class Text;
class Font;
class Button;
class Panel;
class StatusScript;
class Slider;

class Scene {
private:
    Model *girlModel;
    Texture *girlTexture;
    Shader *litVertexShader;
    Shader *litFragmentShader[3];
    Material *girlMaterial[3];
    GameObject *girlGameObject;
    Renderer *girlRenderer;
    Mesh *girlMesh;
    RotateScript *girlRotateScript;

    GameObject *mainCameraGameObject;
    Camera *mainCameraCamera;

    GameObject *lightGameObject;
    Light *lightLight;

    Model *tileModel;
    Texture *tileTexture;
    Material *tileMaterial[3];
    GameObject *tileGameObject;
    Renderer *tileRenderer;
    Mesh *tileMesh;

    StatusScript *statusScript;
    Texture *panelTexture;
    GameObject *panelGameObject;
    Panel *panelPanel;
    Font *arialFont;
    GameObject *fpsTextGameObject;
    Text *fpsTextText;
    GameObject *sizeTextGameObject;
    Text *sizeTextText;
    Texture *buttonTexture;
    GameObject *sizeOptionButtonGameObject[2];
    Button *sizeOptionButtonButton[2];
    GameObject *sizeOptionTextGameObject[2];
    Text *sizeOptionTextText[2];
    GameObject *algorithmTextGameObject;
    Text *algorithmTextText;
    GameObject *algorithmOptionButtonGameObject[3];
    Button *algorithmOptionButtonButton[3];
    GameObject *algorithmOptionTextGameObject[3];
    Text *algorithmOptionTextText[3];
    GameObject *areaTextGameObject;
    Text *areaTextText;
    Texture *handleTexture;
    GameObject *areaSliderGameObject;
    Slider *areaSliderSlider;
    GameObject *viewTextGameObject;
    Text *viewTextText;
    GameObject *viewOptionButtonGameObject[2];
    Button *viewOptionButtonButton[2];
    GameObject *viewOptionTextGameObject[2];
    Text *viewOptionTextText[2];

public:
    Scene();
    virtual ~Scene();
};

#endif