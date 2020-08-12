#ifndef STATUS_SCRIPT_H
#define STATUS_SCRIPT_H

#include "Script.h"

class Text;

class StatusScript : public Script {
private:
    Text *fpsText;

public:
    StatusScript(Text *fpsText);
    virtual ~StatusScript();

    virtual void OnUpdate() override;
};

#endif