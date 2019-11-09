#include <gl00/model.hpp>
#include "../src/sdl/sdl_helper.hpp"

//#include <filesystem>
#include <iostream>
#include <stdio.h>


void ListRenderDrivers()
{
    int numdrivers = SDL_GetNumRenderDrivers();

    printf("Drivers count: %i\n", numdrivers);

    for (int i = 0; i < numdrivers; i++)
    {
        SDL_RendererInfo drinfo;
        SDL_GetRenderDriverInfo(i, &drinfo);

        printf("Driver name: %s\n", drinfo.name);
    }
}

void StartApp()
{
    std::cout << "cout\n";
    printf("printf\n");
    LOGI("LOGI\n");
    LOGD("LOGD\n");
    LOGE("LOGE\n");
    gl00::SDLApp app;

    glClearColor(1.f, 0.f, 0.f, 1.f);

    SDL_Event event;

    for (;;)
    {
        while (SDL_PollEvent(&event)) {
        
            switch (event.type)
            {
            case SDL_QUIT:
                goto quit;
                break;
            }
        }
        app.Swap();
    }

quit:
    app.Quit();
}

int main(int argc, char* argv[])
{
    //namespace fs = std::filesystem;

    //std::cout << std::filesystem::current_path() << std::endl;

    std::cout << "Model Test" << std::endl;

    //gl00::Model m("assets/cube_object.obj");

    //for (int i = 0; i < 5; ++i)
    //    StartApp();


    ListRenderDrivers();
    StartApp();

    std::cout << "End" << std::endl;

    return 0;
}