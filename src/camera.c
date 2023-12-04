#include "camera.h"

Camera camera_create()
{
    Camera cam = {
        .position = {0.0f, 0.0f,  2.0f},
        .up       = {0.0f, 1.0f,  0.0f},

        .target   = {0.0f, 0.0f,  0.0f},
        .front    = {0.0f, 0.0f, -1.0f},

        .speed = 1.0f,
    };
    
    // direction
    glm_vec3_sub(cam.position, cam.target, cam.direction);
    glm_normalize(cam.direction);

    // right
    glm_cross(cam.up, cam.direction, cam.right);
    glm_normalize(cam.right);

    // view
    vec3 center = GLM_VEC3_ZERO_INIT;
    glm_vec3_add(cam.position, cam.front, center);
    glm_lookat(cam.position, center, cam.up, cam.view);

    return cam;
}

void camera_move(Camera* cam, vec3 movement)
{
    // could be done with cglm but I couldn't find a way without excessive code
    cam->position[0] += movement[0];
    cam->position[1] += movement[1];
    cam->position[2] += movement[2];
}

void camera_move_to(Camera* cam, vec3 position)
{
    glm_vec3_copy(position, cam->position);
}
