#include <memory>
#include <gl00/gl00.hpp>
#include "../src/scenes/test_scene.hpp"

#include "../src/sdl/sdl_helper.hpp"

constexpr int WIDTH = 768;
constexpr int HEIGHT = 768;

int main(int argc, char* agv[])
{
    // Test Case 1
    //std::unique_ptr<gl00::scenes::BezierScene> test_scene(new gl00::scenes::BezierScene);

    // Test Case 2
    //std::unique_ptr<TestScene> test_scene(new TestScene);
    //gl00::Engine engine(test_scene.get());

    // Test Case 3
    gl00::Engine engine;

    std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> window 
        = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);

    SDL_GLContext context;
    
    gl00::_SDL_Init(window, context, WIDTH, HEIGHT);

    SDL_Log("%s\n", glGetString(GL_VENDOR));
    SDL_Log("%s\n", glGetString(GL_VERSION));

    SDL_GL_SetSwapInterval(0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height = 0;

    gl00::SceneManager& mgr = gl00::SceneManager::GetInstance();
    SDL_Event e;

    for (;;)
    {
        SDL_GetWindowSize(window.get(), &width, &height);
        engine.SetScreenDimensions(width, height);

        if (!mgr.GetScene())
            goto draw;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    goto exit;
                default:
                    break;
                }
                break;
            case SDL_QUIT:
                goto exit;
            }
        }
draw:
        engine.DoFrame();
        SDL_GL_SwapWindow(window.get());
    }
exit:
    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}