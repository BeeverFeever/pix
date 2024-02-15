#include <GLFW/glfw3.h>
#include <cglm/vec3.h>
#include <math.h>
#include "camera.h"

static int lastx = 0;
static int lasty = 0;
static float yaw = 0;
static float pitch = 0;

/* static void update_vectors(Camera* cam) */
/* { */
/*     vec3 front; */
/*     front[0] = cos(glm_rad(cam->yaw)); */
/**/
/*     // direction */
/*     glm_vec3_sub(cam->position, cam->target, cam->direction); */
/*     glm_normalize(cam->direction); */
/**/
/*     // right */
/*     glm_cross(cam->cam_up, cam->direction, cam->right); */
/*     glm_normalize(cam->right); */
/* } */

void camera_init(Camera* cam)
{
    glm_vec3_zero(cam->position);
    cam->position[2] = 2.0f;

    glm_vec3_zero(cam->cam_up);
    cam->cam_up[1] = 1.0f;

    glm_vec3_zero(cam->target);

    glm_vec3_zero(cam->front);
    cam->front[2] = -1.0f;

    cam->speed = 1.0f;
    cam->fov = 45.0f;

    // direction
    glm_vec3_sub(cam->position, cam->target, cam->direction);
    glm_normalize(cam->direction);

    // right
    glm_cross(cam->cam_up, cam->direction, cam->right);
    glm_normalize(cam->right);

    // view
    camera_update_view_matrix(cam);

    /* update_vectors(cam); */
}

void camera_move_direction(Camera* cam, CameraDirection dir)
{
    switch(dir) {
        case FORWARD:
            glm_vec3_mulsubs(cam->direction, cam->speed, cam->position);
            break;
        case BACK:
            glm_vec3_muladds(cam->direction, cam->speed, cam->position);
            break;
        case UP:
            glm_vec3_muladds(cam->cam_up, cam->speed, cam->position);
            break;
        case DOWN:
            glm_vec3_mulsubs(cam->cam_up, cam->speed, cam->position);
            break;
        case LEFT: {
            vec3 cross = GLM_VEC3_ZERO_INIT;
            glm_vec3_cross(cam->direction, cam->cam_up, cross);
            glm_vec3_normalize(cross);
            glm_vec3_muladds(cross, cam->speed, cam->position);
            break;
        }
        case RIGHT: {
            vec3 cross = GLM_VEC3_ZERO_INIT;
            glm_vec3_cross(cam->direction, cam->cam_up, cross);
            glm_vec3_normalize(cross);
            glm_vec3_mulsubs(cross, cam->speed, cam->position);
        }
        default:
            break;
    }
}

void camera_keyboard_callback(GLFWwindow* win, Camera* cam, float dt)
{
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_move_direction(cam, UP);
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_move_direction(cam, DOWN);

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        camera_move_direction(cam, FORWARD);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        camera_move_direction(cam, BACK);

    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        camera_move_direction(cam, LEFT);
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        camera_move_direction(cam, RIGHT);
}

void camera_mouse_callback(GLFWwindow* win, Camera* cam, double xpos, double ypos)
{
    static bool firstmouse = true;
    if (firstmouse) {
        lastx = xpos;
        lasty = ypos;
        firstmouse = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos;
    lastx = xpos;
    lasty = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction = GLM_VEC3_ZERO_INIT;
    direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    direction[1] = sin(glm_rad(pitch));
    direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
    glm_vec3_normalize(direction);
    glm_vec3_copy(direction, cam->front);
}

void camera_update_view_matrix(Camera* cam)
{
    vec3 temp = GLM_VEC3_ZERO_INIT;
    glm_vec3_add(cam->position, cam->front, temp);
    glm_lookat(cam->position, temp, cam->cam_up, cam->view);
}
