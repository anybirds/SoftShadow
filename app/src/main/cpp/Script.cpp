#include "Script.h"

using namespace std;

unordered_set<Script *> Script::scripts;

void Script::Start() {
    for (Script *script : scripts) {
        script->OnStart();
    }
}

void Script::Update() {
    for (Script *script : scripts) {
        script->OnUpdate();
    }
}

Script::Script() {
    scripts.insert(this);
}

Script::~Script() {
    scripts.erase(this);
}