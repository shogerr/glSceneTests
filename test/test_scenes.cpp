#include <iostream>
#include <memory>

#include <gl00/scene.hpp>
#include <gl00/engine.hpp>
#include <gl00/scene_manager.hpp>

#include "../src/sdl/sdl_helper.hpp"

#include "../src/scenes/blank.hpp"
#include "../src/scenes/bezier_scene.hpp"
#include "../src/scenes/line_test.hpp"
#include "../src/scenes/shader_scene.hpp"
#include "../src/scenes/texture_scene.hpp"

#ifdef _WIN32
#include <windows.h>
#endif
#include <glad/glad.h>

int main(int argc, char* argv[])
{
    gl00::SDLApp app;
    gl00::Engine engine;

    //std::unique_ptr<gl00::Scene> s { new gl00::scenes::LineTest };
    //engine.SetDefaultScene(std::move(s));

    auto& mgr = gl00::SceneManager::GetInstance();

    SDL_Event event;

    for (;;)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_F1:
                    mgr.RequestNewScene(std::make_unique<gl00::scenes::Blank>());
                    break;
                case SDLK_F2:
                    mgr.RequestNewScene(std::make_unique<gl00::scenes::BezierScene>());
                    break;
                case SDLK_F3:
                    mgr.RequestNewScene(std::make_unique<gl00::scenes::LineTest>());
                    break;
                case SDLK_F4:
                    mgr.RequestNewScene(std::make_unique<gl00::scenes::ShaderScene>());
                    break;
                case SDLK_F5:
                    mgr.RequestNewScene(std::make_unique<gl00::scenes::TextureScene>());
                    break;
                }
                break;
            case SDL_QUIT:
                goto exit;
            }
        }
        engine.DoFrame();
        app.Swap();
    }
exit:
    app.Quit();
    return 0;
}