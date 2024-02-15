#pragma once

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef enum {
    FORWARD,
    BACK,
    UP,
    DOWN,
    LEFT,
    RIGHT,
} CameraDirection;

typedef struct {
    vec3 position;
    vec3 direction;
    vec3 right;
    vec3 cam_up;
    vec3 world_up;

    // euler angles
    float yaw;
    float pitch;

    align mat4 view;

    vec3 target;
    vec3 front;

    float speed;
    float sensitivity;
    float fov;
} Camera;

void camera_init(Camera* cam);
void camera_move_direction(Camera* cam, CameraDirection dir);
void camera_update_view_matrix(Camera* cam);

void camera_keyboard_callback(GLFWwindow* win, Camera* cam, float dt);
void camera_mouse_callback(GLFWwindow* win, Camera* cam, double xpos, double ypos);
