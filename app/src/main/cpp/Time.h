#ifndef TIME_H
#define TIME_H

class Time {
public:
    static void Init();
    static void Tick();
    static float DeltaTime() { return dt; }

private:
    static float dt;
    static float prev;

public:
    Time() = delete;
};

#endif