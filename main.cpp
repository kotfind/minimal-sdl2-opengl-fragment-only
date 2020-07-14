// Minimal SDL2 + OpenGL fragment shader only example
//
// Author: https://qithub.com/kotfind
//
// This code is in the public domain. In other words, you can
// do whatever you want with it. Hope, it would be helpful.
//
// Compilation: g++ main.cpp -lGL -lSDL2
// Runing:      ./a.out
//
// This code was tested on
// OS: Arch Linux x86_64
// Kernel: 5.7.6-arch1-1
// WM: i3
// CPU: Intel i5-3570 (4) @ 3.800GHz
// GPU: NVIDIA GeForce GT 710



#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>
using namespace std;

int main(int argc, char **argv) {
    // SDL2 init
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *win = SDL_CreateWindow("",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_GLContext cont = SDL_GL_CreateContext(win);
    
    // Load shader code from file
    FILE *fragment_shader_fp = fopen("shader.frag", "r");
    fseek(fragment_shader_fp, 0L, SEEK_END);
    long int fragment_shader_fs = ftell(fragment_shader_fp) + 1;
    rewind(fragment_shader_fp);

    char *fragment_shader = (char*) malloc(fragment_shader_fs * sizeof(char));
    for (int i = 0; i < fragment_shader_fs; ++i) {
        fragment_shader[i] = fgetc(fragment_shader_fp);
    }
    fragment_shader[fragment_shader_fs - 1] = 0;

    fclose(fragment_shader_fp);

    // Shader
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, (const GLchar**)&fragment_shader, NULL);
    glCompileShader(shader);

    // Check shader for errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

        char *errlog = (char*) malloc(len * sizeof(char));
        glGetShaderInfoLog(shader, len, &len, errlog);

        cerr << errlog << '\n';

        glDeleteShader(shader);
        SDL_GL_DeleteContext(cont);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return -1;
    }

    // Shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);
    glUseProgram(program);

    // Get uniform locations
    GLuint uTime = glGetUniformLocation(program, "time");
    GLuint uResolution = glGetUniformLocation(program, "resolution");

    glDisable(GL_DEPTH_TEST);

    // Mainloop
    SDL_Event e;
    for(;;) {
        bool quit = 0;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        if (quit) break;

        // Set the time uniform
        float time = SDL_GetTicks();
        glUniform1f(uTime, time);

        // Set the resolution uniform
        int W, H;
        SDL_GetWindowSize(win, &W, &H);
        glViewport(0, 0, W, H);
        glUniform2i(uResolution, W, H);

        glClear(GL_COLOR_BUFFER_BIT);
        glRectf(-1., -1., 1., 1.);

        SDL_GL_SwapWindow(win);
    }

    // Quit
    SDL_GL_DeleteContext(cont);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
