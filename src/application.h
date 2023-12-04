#pragma once

#include "camera.h"

typedef struct {
    char* name;
    Camera main_cam;

    float dt;
    float last_frame;
} Application;

Application app_create(char* name);
