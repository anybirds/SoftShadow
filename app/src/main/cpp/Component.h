#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component {
private:
    GameObject *gameObject;

public:
    Component();
    virtual ~Component();

    GameObject *GetGameObject() const { return gameObject; }
    void SetGameObject(GameObject *gameObject) { this->gameObject = gameObject; }
};

#endif