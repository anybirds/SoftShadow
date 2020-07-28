#ifndef TIME_H
#define TIME_H

#include <time.h>

class Time {
public:
    static void Init();
    static void Tick();
    static float DeltaTime() { return dt; }
    static void StartTimer();
    static float StopTimer();

private:
    static float dt;
    static float prev;
    static timespec res;
    static float timer;
public:
    Time() = delete;
};

#endif