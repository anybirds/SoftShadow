#ifndef SCENE_H
#define SCENE_H

class Camera;
class GameObject;
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
    Shader *unlitVertexShader;
    Shader *unlitFragmentShader;
    Material *girlMaterial;
    GameObject *girlGameObject;
    Renderer *girlRenderer;
    Mesh *girlMesh;
    GameObject *mainCameraGameObject;
    Camera *mainCameraCamera;
    RotateScript *girlRotateScript;

public:
    Scene();
    virtual ~Scene();
};

#endif