#pragma once

#include "glad.h"

typedef GLuint Shader;
typedef GLuint ShaderProgram;

ShaderProgram shaderprogram_create(char* vert_path, char* frag_path);
void shaderprogram_activate(ShaderProgram program);
void shaderprogram_free(ShaderProgram program);

void uniform_set_int(ShaderProgram program, char* name, int32_t value);
void uniform_set_uint(ShaderProgram program, char* name, uint32_t value);
void uniform_set_float(ShaderProgram program, char* name, float value);
void uniform_set_bool(ShaderProgram program, char* name, bool value);
void uniform_set_double(ShaderProgram program, char* name, double value);

void uniform_set_vec2(ShaderProgram program, char* name, float* value);
void uniform_set_vec3(ShaderProgram program, char* name, float* value);
void uniform_set_vec4(ShaderProgram program, char* name, float* value);

void uniform_set_mat2(ShaderProgram program, char* name, float* value);
void uniform_set_mat3(ShaderProgram program, char* name, float* value);
void uniform_set_mat4(ShaderProgram program, char* name, float* value);
