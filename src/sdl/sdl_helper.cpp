#include "sdl_helper.hpp"

//#include <gl00/common.hpp>

#include <fstream>

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

void gl00::_SDL_Init(std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>& window, SDL_GLContext& context, int width, int height)
{
    SDLInit(window, context, width, height);
}

void gl00::SDLInit(std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> & window, SDL_GLContext &context, int width, int height)
{
    SDL_GLContext context_;

    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status < 0)
    {
        std::ofstream stdo{"stdout.txt"};
        auto e = SDL_GetError();
        SDL_Log("SDL Error: %s.\n", e);
        SDL_Quit();
        stdo << e << "\n";
        exit(1);
    }

    window.reset(SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));

    if (!window) {
        SDL_Log("%s:\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    context_ = SDL_GL_CreateContext(window.get());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

#if defined(__linux__)
    // The old GLEW method.
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
        SDL_Log("Error initializing GLEW.  %s\n", glewGetErrorString(glewError));
#else
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        exit(1);
    }
#endif

    context = context_;
    gl00::_SDL_CheckError(__LINE__);
}
