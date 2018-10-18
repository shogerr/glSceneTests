#include <iostream>
#include <SDL.h>

#include "common.hpp"
#include "engine.hpp"
#include "scene_manager.hpp"
#include "helicopter_scene.hpp"

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;


void checkSDLError(int line = -1)
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

void init(SDL_Window *&window, SDL_GLContext &context)
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

    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("%s:\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    context_ = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
        std::cout << "Error initializing GLEW. " << glewGetErrorString(glewError);

    context = context_;
    checkSDLError(__LINE__);
}


int main(int argc, char *argv[])
{
    Engine* engine = new Engine;

    SDL_Window *window = NULL;
    SDL_GLContext context;
    
    init(window, context);

    SDL_Log("%s\n", glGetString(GL_VENDOR));
    SDL_Log("%s\n", glGetString(GL_VERSION));

    SDL_GL_SetSwapInterval(1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height = 0;

    SceneManager* mgr = SceneManager::getInstance();
    SDL_Event e;
    for (;;)
    {
        if (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
                LOGI("view change\n");
                static_cast<HelicopterScene*>(mgr->getScene())->toggleView();
                break;
            case SDL_QUIT:
                goto exit;
            }
        }
        
        SDL_GetWindowSize(window, &width, &height);
        engine->setScreenDimensions(width, height);

        engine->cycle();
        SDL_GL_SwapWindow(window);
    }

exit:
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}