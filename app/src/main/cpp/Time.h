#ifndef TIME_H
#define TIME_H

#include <time.h>

class Time {
public:
    static void Init();
    static void Tick();
    static float DeltaTime() { return dt; }

private:
    static float dt;
    static float prev;
    static timespec res;

public:
    Time() = delete;
};

#endif