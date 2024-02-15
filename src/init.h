#pragma once
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void create_context(GLFWwindow* win);
GLFWwindow* init_window(int w, int h, const char* name);
