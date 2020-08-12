#include "Time.h"

float Time::dt;
float Time::prev;
struct timespec Time::res;
int Time::fps;
float Time::fps_prev;
int Time::fps_cnt;

void Time::Init() {
    dt = 0.0f;
    clock_gettime(CLOCK_MONOTONIC, &res);
    prev = (float)(res.tv_sec + (double)res.tv_nsec / 1e9);

    fps = 0;
    fps_cnt = 0;
    fps_prev = prev;
}

void Time::Tick() {
    clock_gettime(CLOCK_MONOTONIC, &res);
    float curr = (float)(res.tv_sec + (double)res.tv_nsec / 1e9);
    dt = curr - prev;
    prev = curr;

    fps_cnt++;
    if (curr > fps_prev + 1.0f) {
        fps = (int)(1.0f / ((curr - fps_prev) / fps_cnt));
        fps_cnt = 0;
        fps_prev = curr;
    }
}