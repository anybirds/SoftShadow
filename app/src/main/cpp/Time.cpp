#include "Time.h"

float Time::dt;
float Time::prev;
struct timespec Time::res;
float Time::timer;

void Time::Init() {
    dt = 0.0f;
    clock_gettime(CLOCK_MONOTONIC, &res);
    prev = (float)(res.tv_sec + (double)res.tv_nsec / 1e9);
}

void Time::Tick() {
    clock_gettime(CLOCK_MONOTONIC, &res);
    float curr = (float)(res.tv_sec + (double)res.tv_nsec / 1e9);
    dt = curr - prev;
    prev = curr;
}

void Time::StartTimer() {
    clock_gettime(CLOCK_MONOTONIC, &res);
    timer = (float)(res.tv_sec + (double)res.tv_nsec / 1e9);
}

float Time::StopTimer() {
    clock_gettime(CLOCK_MONOTONIC, &res);
    float curr = (float)(res.tv_sec + (double)res.tv_nsec / 1e9);
    return curr - timer;
}