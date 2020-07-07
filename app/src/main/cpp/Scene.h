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

class Scene {
private:
    Model *triModel;
    Texture *triTexture;
    Shader *unlitVertexShader;
    Shader *unlitFragmentShader;
    Material *triMaterial;
    GameObject *triGameObject;
    Renderer *triRenderer;
    Mesh *triMesh;
    GameObject *mainCameraGameObject;
    Camera *mainCameraCamera;

public:
    Scene::Scene();
    virtual ~Scene();
};

#endif