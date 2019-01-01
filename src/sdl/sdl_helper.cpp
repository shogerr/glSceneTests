#include <gl00/common.hpp>
#include "sdl_helper.hpp"

void gl00::_SDL_CheckError(int line)
{
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
        SDL_Log("SDL Error: %s\n", error);
        if (line != -1)
            SDL_Log("SDL Error: %i\n", line);
        SDL_ClearError();
    }
}

void gl00::_SDL_Init(SDL_Window *&window, SDL_GLContext &context, int width, int height)
{
    SDL_GLContext context_;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("%s:\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("%s:\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    context_ = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
        LOGI("Error initializing GLEW.  %s\n", glewGetErrorString(glewError));

    context = context_;
    gl00::_SDL_CheckError(__LINE__);
}
