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

class Scene {
private:
    Model *girlModel;
    Texture *girlTexture;
    Shader *litVertexShader;
    Shader *litFragmentShader;
    Material *girlMaterial;
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
    Material *tileMaterial;
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

public:
    Scene();
    virtual ~Scene();
};

#endif