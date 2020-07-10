#include <time.h>

#include "Time.h"

float Time::dt;
float Time::prev;

void Time::Init() {
    dt = 0.0f;
    prev = (float)clock() / CLOCKS_PER_SEC;
}

void Time::Tick() {
    float curr = (float)clock() / CLOCKS_PER_SEC;
    dt = curr - prev;
    prev = curr;
}