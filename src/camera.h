#pragma once

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 right;
    vec3 up;

    mat4 align view;

    vec3 target;
    vec3 front;

    float speed;
} Camera;

Camera camera_create();
void camera_move(Camera* cam, vec3 movement);
void camera_move_to(Camera* cam, vec3 position);
