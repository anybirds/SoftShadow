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
    GameObject *mainCameraGameObject;
    Camera *mainCameraCamera;
    RotateScript *girlRotateScript;
    GameObject *lightGameObject;
    Light *lightLight;
    Model *tileModel;
    Texture *tileTexture;
    Material *tileMaterial;
    GameObject *tileGameObject;
    Renderer *tileRenderer;
    Mesh *tileMesh;
    GameObject *textGameObject;
    Text *textText;
    Font *arialFont;
    Texture *buttonTexture;
    GameObject *buttonGameObject;
    Button *buttonButton;
    Texture *panelTexture;
    GameObject *panelGameObject;
    Panel *panelPanel;
    StatusScript *statusScript;

public:
    Scene();
    virtual ~Scene();
};

#endif