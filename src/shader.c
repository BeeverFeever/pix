#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

static FILE* _open_file(char* path, char* mode);
static char* _read_file(char* path);
static Shader _load_shader(GLenum type, char* path);

ShaderProgram shaderprogram_create(char* vert_path, char* frag_path) {
    // create both shaders
    Shader vert = _load_shader(GL_VERTEX_SHADER, vert_path);
    Shader frag = _load_shader(GL_FRAGMENT_SHADER, frag_path);

    // create and link program
    ShaderProgram program;
    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    // error check the program
    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "ERROR: shader program linking\n%s", info_log);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

void shaderprogram_activate(ShaderProgram program) {
    glUseProgram(program);
}

void shaderprogram_free(ShaderProgram program) {
    glDeleteProgram(program);
}

// Uniforms
void uniform_set_int(ShaderProgram program, char* name, i32 value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}
void uniform_set_uint(ShaderProgram program, char* name, u32 value) {
    glUniform1ui(glGetUniformLocation(program, name), value);
}
void uniform_set_float(ShaderProgram program, char* name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}
void uniform_set_bool(ShaderProgram program, char* name, bool value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}
void uniform_set_double(ShaderProgram program, char* name, double value) {
    glUniform1d(glGetUniformLocation(program, name), value);
}

void uniform_set_vec2(ShaderProgram program, char* name, float* value) {
    glUniform2fv(glGetUniformLocation(program, name), 1, value);
}
void uniform_set_vec3(ShaderProgram program, char* name, float* value) {
    glUniform3fv(glGetUniformLocation(program, name), 1, value);
}
void uniform_set_vec4(ShaderProgram program, char* name, float* value) {
    glUniform3fv(glGetUniformLocation(program, name), 1, value);
}

void uniform_set_mat2(ShaderProgram program, char* name, float* value) {
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}
void uniform_set_mat3(ShaderProgram program, char* name, float* value) {
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}
void uniform_set_mat4(ShaderProgram program, char* name, float* value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}






// TODO: move these into seperate file.

static FILE* _open_file(char* path, char* mode) {
    FILE* in = fopen(path, mode);
    if (in == NULL) {
        fprintf(stderr, "ERROR: could not open %s\n%s", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return in;
}

static char* _read_file(char* path) {
    FILE* file = _open_file(path, "r");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(length + 1);
    if (buffer == NULL) {
        fprintf(stderr, "ERROR: malloc error. %i:%s", __LINE__, __FILE__);
        exit(1);
    }
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';
    return buffer;
}

static Shader _load_shader(GLenum type, char* path) {
    // read in and compile shader
    Shader shader = glCreateShader(type);
    char* source = _read_file(path);
    glShaderSource(shader, 1, (const GLchar* const*)&source, NULL);
    glCompileShader(shader);

    // check for compilation errors
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR: shader compilation: %s\n%s", path, info_log);
    }

    free(source);
    return shader;
}

