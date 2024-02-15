#include "glad.h"
#include <GLFW/glfw3.h>

#define CGLM_DEFINE_PRINTS
#include <cglm/cglm.h>

#include "shader.h"
#include "camera.h"
#include "init.h"

float dt = 0;
float last_frame = 0;

Camera cam;

void process_input(GLFWwindow* win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

    camera_keyboard_callback(win, &cam, dt);
}

void process_mouse(GLFWwindow* win)
{
    camera_mouse_callback(win, &cam, )
}

int main(void)
{
    int width  = 1920;
    int height = 1080;

    GLFWwindow* win = init_window(width, height, "testing");
    create_context(win);
    camera_init(&cam);

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

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shaderprogram_activate(shader_program);

    mat4 align proj  = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(cam.fov), (float)width / (float)height, 0.1f, 100.0f, proj);
    uniform_set_mat4(shader_program, "proj", *proj);

    cam.speed = 0.2f;
    while (!glfwWindowShouldClose(win)) {
        // delta time
        float current_time = glfwGetTime();
        dt = current_time - last_frame;
        last_frame = current_time;

        // input
        process_input(win);

        glClearColor(0.8f, 0.8f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ----- program in here ----- //
        shaderprogram_activate(shader_program);

        camera_update_view_matrix(&cam);
        uniform_set_mat4(shader_program, "view", *cam.view);

        mat4 align model = GLM_MAT4_IDENTITY_INIT;
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
