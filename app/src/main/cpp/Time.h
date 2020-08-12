#ifndef TIME_H
#define TIME_H

#include <time.h>

class Time {
public:
    static void Init();
    static void Tick();
    static float DeltaTime() { return dt; }
    static int Fps() { return fps; }

private:
    static float dt;
    static float prev;
    static int fps;
    static float fps_prev;
    static int fps_cnt;
    static timespec res;

public:
    Time() = delete;
};

#endif