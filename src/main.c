#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include "application.h"
#include "shader.h"

void glm_vec3_mulsub(vec3 a, float s, vec3 dest) {
  dest[0] -= a[0] * s;
  dest[1] -= a[1] * s;
  dest[2] -= a[2] * s;
}


typedef struct {
    float verticies;
    int   indicies;
} Model;

void process_input(GLFWwindow* window, Application* app) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glm_vec3_muladds(app->main_cam.front, app->main_cam.speed, app->main_cam.position);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glm_vec3_mulsub(app->main_cam.front, app->main_cam.speed, app->main_cam.position);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        glm_vec3_muladds(app->main_cam.right, app->main_cam.speed, app->main_cam.position);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        glm_vec3_mulsub(app->main_cam.right, app->main_cam.speed, app->main_cam.position);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void create_context(GLFWwindow* win) {
    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialise GLEW\n");
        exit(1);
    }

    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}

GLFWwindow* init_window(int w, int h, const char* name) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialise GLFW\n");
        exit(1);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* win;
    win = glfwCreateWindow(w, h, name, NULL, NULL);
    if (win == NULL) {
        fprintf(stderr, "Failed to open glfw window\n");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialise GLEW\n");
        exit(1);
    }

    glViewport(0, 0, h, w);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

    return win;
}

int main(void) 
{
    Application app = app_create("pix - opengl testing");

    int width  = 800;
    int height = 600;

    GLFWwindow* win = init_window(width, height, "testing");
    create_context(win);

    ShaderProgram shader_program = shaderprogram_create("shaders/vertshader.glsl", "shaders/fragshader.glsl");

    float cube_vertices[] = {
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
    };

    // These may seem somewhat random but it is a triangle strip rather than
    // specifying all seperate triangles. I am working on writing an article
    // about this.
    uint cube_elements[] = {
        0,1,2,
        3,6,7,
        4,5,0,
        1,5,1,
        7,3,0,
        4,2,6,
    };

    float vert_colours[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };


    uint vao, vbo, ebo;

    // generate the buffers 
    glGenBuffers(1, &vbo); 
    glGenBuffers(1, &ebo); 
    glGenVertexArrays(1, &vao); 

    glBindVertexArray(vao); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW); 

    int pos_attr = 0;

    // position attribue
    glVertexAttribPointer(pos_attr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(pos_attr);

    // colour attribute
    uint colourbuffer;
    glGenBuffers(1, &colourbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_colours), vert_colours, GL_STATIC_DRAW);

    int colour_attr = 1;
    glVertexAttribPointer(colour_attr, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(colour_attr);

    /* vec3 cam_pos = {0.0f, 0.0f, 3.0f}; */
    /* vec3 cam_target = {0.0f, 0.0f, 0.0f}; */
    /* vec3 cam_direction;  */
    /* glm_vec3_sub(cam_pos, cam_target, cam_direction); */
    /* glm_vec3_normalize(cam_direction); */
    /**/
    /* vec3 up = {0.0f, 1.0f, 0.0f}; */
    /* vec3 cam_right; */
    /* glm_vec3_cross(up, cam_direction, cam_right); */
    /* glm_vec3_normalize(cam_right); */
    /**/
    /* vec3 cam_up; */
    /* glm_vec3_cross(up, cam_direction, cam_up); */
    /**/

    int fov = 45.0f;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shaderprogram_activate(shader_program);

    mat4 align proj  = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(fov), (float)width / (float)height, 0.1f, 100.0f, proj);
    uniform_set_mat4(shader_program, "proj", *proj);

    while (!glfwWindowShouldClose(win)) {
        // delta time
        float current_time = glfwGetTime();
        app.dt = current_time - app.last_frame;
        app.last_frame = current_time;

        // make camera speed the same on all devices
        /* app.main_cam.speed *= app.dt; */

        process_input(win, &app);

        glClearColor(0.8f, 0.8f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ----- program in here ----- // 
        shaderprogram_activate(shader_program);

        mat4 align model = GLM_MAT4_IDENTITY_INIT;
        /* mat4 align view  = GLM_MAT4_IDENTITY_INIT; */

        glm_lookat(app.main_cam.position, app.main_cam.target, app.main_cam.up, app.main_cam.view);
        /* glm_translate(cam.view, (vec3){0.0f, 0.0f, -3.0f}); */
        /* uniform_set_mat4(shader_program, "view", cam.view); */
        /* glm_translate(model, (vec3){0.0f, 0.0f, 0.0f}); */
        /* uniform_set_mat4(shader_program, "model", model); */

        uniform_set_mat4(shader_program, "view", *app.main_cam.view);

        // --- object transformations here
        /* glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f}); */
        // ---
        uniform_set_mat4(shader_program, "model", *model);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_INT, 0);


        // --------------------------- //


        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    shaderprogram_free(shader_program);
    glfwTerminate();
    return 0;
}
