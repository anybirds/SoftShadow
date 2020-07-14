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
    Shader *shadowMapVertexShader;
    Shader *shadowMapFragmentShader;
    Material *shadowMapMaterial;
    Model *tileModel;
    Texture *tileTexture;
    Material *tileMaterial;
    GameObject *tileGameObject;
    Renderer *tileRenderer;
    Mesh *tileMesh;


public:
    Scene();
    virtual ~Scene();
};

#endif