#include <SDL2/SDL.h>
#include <GL/glew.h>
//#include <GL/gl.h>
#include <SDL2/SDL_opengl.h>
#include <memory>

constexpr auto width = 768;
constexpr auto height = 768;

int main(int argc, char* argb[])
{

    auto window = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);


    SDL_Log("SDL Start.\n");

    int status = SDL_Init(SDL_INIT_VIDEO);
    if (status < 0)
    {
        auto e = SDL_GetError();
        SDL_Log("SDL Error: %s\n", e);
        exit(-1);
    }

    window.reset(SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE));
 
    if (!window) {
        SDL_Log("%s:\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    SDL_GLContext context = SDL_GL_CreateContext(window.get());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
        SDL_Log("Error initializing GLEW.  %s\n", glewGetErrorString(glewError));

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window.get());

    SDL_Event event;
    for (;;)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                SDL_Quit();
                goto exit;
                break;
            }
        }
    }

exit:
    return 10;
}