#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject *gameObject) : gameObject(gameObject) {
    gameObject->components.insert({})
}

Component::~Component() {

}