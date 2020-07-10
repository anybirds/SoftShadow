#ifndef SCRIPT_H
#define SCRIPT_H

#include <unordered_set>

#include "Component.h"

class Script : public Component {
private:
    static std::unordered_set<Script *> scripts;

public:
    static void Start();
    static void Update();

public:
    Script();
    virtual ~Script();

    virtual void OnStart() {}
    virtual void OnUpdate() {}
};

#endif