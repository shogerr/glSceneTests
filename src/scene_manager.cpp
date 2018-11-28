#include <scene_manager.hpp>
#include <scene.hpp>
#include <common.hpp>

static SceneManager g_scene_manager;

SceneManager::SceneManager()
{
    current_scene_ = NULL;
    scene_to_install_ = NULL;

    screen_width_ = 512;
    screen_height_ = 512;

    has_graphics_ = false;
}

SceneManager* SceneManager::GetInstance()
{
    return &g_scene_manager;
}

void SceneManager::RequestNewScene(Scene *newScene)
{
    scene_to_install_ = newScene;
} 

Scene* SceneManager::GetScene()
{
    return current_scene_;
}

void SceneManager::InstallScene(Scene* newScene)
{
    bool had_graphics = has_graphics_;
    if (has_graphics_)
        KillGraphics();

    if (current_scene_)
    {
        current_scene_->OnUnInstall();
        delete current_scene_;
        current_scene_ = NULL;
    }

    current_scene_ = newScene;
    if (current_scene_)
        current_scene_->OnInstall();

    if (had_graphics)
        StartGraphics();
}

void SceneManager::StartGraphics()
{
    if (!has_graphics_)
    {
        LOGI("SceneManager: starting graphics.\n");
        has_graphics_ = true;
        if (current_scene_)
        {
            LOGI("Calling OnStartGraphics()\n");
            current_scene_->OnStartGraphics();
        }
    }
}

void SceneManager::KillGraphics() {
    if (has_graphics_) {
        LOGI("SceneManager: killing graphics.\n");
        has_graphics_ = false;
        if (current_scene_) {
            current_scene_->OnKillGraphics();
        }
    }
}

void SceneManager::Poke(unsigned int i)
{
    current_scene_->Poke(i);
}

void SceneManager::UpdateMouseMotion(float x, float y)
{
    current_scene_->OnMouseMotion(x, y);
}

void SceneManager::UpdateJoy(float x, float y)
{
    current_scene_->OnJoy(x, y);
}

void SceneManager::SetScreenSize(int width, int height)
{
    if (screen_width_ != width || screen_height_ != height)
    {
        screen_width_ = width;
        screen_height_ = height;

        if (current_scene_)
            current_scene_->OnScreenResize(width, height);
    }
}

float SceneManager::GetScreenAspect()
{
    return screen_width_ / (float)screen_height_;
}

void SceneManager::DoFrame()
{
    if (scene_to_install_)
    {
        LOGI("Scene to install.\n");
        InstallScene(scene_to_install_);
        scene_to_install_ = NULL;
    }

    if (has_graphics_ && current_scene_)
        current_scene_->DoFrame();
}
