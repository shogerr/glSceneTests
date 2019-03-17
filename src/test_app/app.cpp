#include <gl00/common.hpp>
#include <gl00/engine.hpp>
#include <gl00/scene_manager.hpp>

#include "../sdl/sdl_helper.hpp"

#include "../scenes/shader_scene.hpp"
#include "../scenes/ball_scene.hpp"
#include "../scenes/bezier_scene.hpp"
#include "../scenes/instance_test.hpp"
#include "../scenes/lighting_scene.hpp"
#include "../scenes/scattering_scene.hpp"

constexpr auto WIDTH = 768;
constexpr auto HEIGHT = 768;

int main(int argc, char *argv[])
{
    gl00::Engine* engine = new gl00::Engine;

    std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> window 
        = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);

    SDL_GLContext context;
    
    gl00::_SDL_Init(window, context, WIDTH, HEIGHT);

    SDL_Log("%s\n", glGetString(GL_VENDOR));
    SDL_Log("%s\n", glGetString(GL_VERSION));

    SDL_GL_SetSwapInterval(1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    int width, height = 0;

    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();
    SDL_Event e;

    for (;;)
    {
        SDL_GetWindowSize(window.get(), &width, &height);
        engine->SetScreenDimensions(width, height);

        if (!mgr->GetScene())
            goto draw;

        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
                mgr->UpdateMouseMotion((float)e.motion.x/width, (float)e.motion.y/height);
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_F1:
                    mgr->RequestNewScene(new BallScene);
                    break;
                case SDLK_F2:
                    mgr->RequestNewScene(new ShaderScene);
                    break;
                case SDLK_F3:
                    mgr->RequestNewScene(new gl00::scenes::BezierScene);
                    break;
                case SDLK_F4:
                    mgr->RequestNewScene(new gl00::scenes::InstanceTest);
                    break;
                case SDLK_F5:
                    mgr->RequestNewScene(new LightingScene);
                    break;
                case SDLK_F6:
                    mgr->RequestNewScene(new gl00::scenes::ScatteringScene);
                    break;
                case SDLK_0:
                    mgr->Poke(0);
                    break;
                case SDLK_1:
                    mgr->Poke(1);
                    break;
                case SDLK_2:
                    mgr->Poke(2);
                    break;
                case SDLK_3:
                    mgr->Poke(3);
                    break;
                case SDLK_4:
                    mgr->Poke(4);
                    break;
                case SDLK_5:
                    mgr->Poke(5);
                    break;
                case SDLK_6:
                    mgr->Poke(6);
                    break;
                case SDLK_w:
                    mgr->Poke(SDLK_w);
                    break;
                case SDLK_s:
                    mgr->Poke(SDLK_s);
                    break;
                case SDLK_a:
                    mgr->Poke(SDLK_a);
                    break;
                case SDLK_d:
                    mgr->Poke(SDLK_d);
                    break;
                case SDLK_b:
                    mgr->Poke(SDLK_b);
                    break;
                }
                break;
            case SDL_QUIT:
                goto exit;
            }
        }
        
draw:
        engine->DoFrame();
        SDL_GL_SwapWindow(window.get());
    }

exit:
    SDL_GL_DeleteContext(context);
    //SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}