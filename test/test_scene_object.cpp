#include "../src/sdl/sdl_helper.hpp"
#include <engine.hpp>
#include <scene_object.hpp>
#include <model.hpp>

constexpr auto WIDTH = 512;
constexpr auto HEIGHT = 512;

SDL_Window *window = NULL;
SDL_GLContext context;

void StartSDL()
{
    gl00::_SDL_Init(window, context, WIDTH, HEIGHT);

    SDL_Log("%s\n", glGetString(GL_VENDOR));
    SDL_Log("%s\n", glGetString(GL_VERSION));

    SDL_GL_SetSwapInterval(1);
}

void TestSceneObject()
{
    gl00::Model* p_i = new gl00::Model{ "../../src/test/cube_object.obj", 1};

    gl00::SceneObject<gl00::Model>* i = new gl00::SceneObject<gl00::Model>{ p_i };

    printf("%p\n", i->t_);

    delete i->t_;

    i->t_ = NULL;

    printf("%p\n", i->t_);
}

void RunSDL()
{
    gl00::Engine* engine = new gl00::Engine{};
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height = 0;
    int mouse_x, mouse_y;

    SDL_Event e;

    for (;;)
    {
        if (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_q:
                    SDL_Quit();
                    break;
                }
                break;
            case SDL_QUIT:
                goto exit;
            }
        }
        
        SDL_GetWindowSize(window, &width, &height);
        engine->DoFrame();

        SDL_GL_SwapWindow(window);
    }

exit:
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char* argv[])
{
    StartSDL();
    TestSceneObject();
    RunSDL();

    return 0;
}