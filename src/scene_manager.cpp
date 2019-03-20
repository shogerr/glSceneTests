#include <gl00/scene_manager.hpp>
#include <gl00/scene.hpp>

static gl00::SceneManager g_scene_manager{};

gl00::SceneManager::SceneManager() :
    current_scene_(nullptr),
    scene_to_install_(nullptr),
    screen_width_(640),
    screen_height_(480),
    has_graphics_(false)
{}

gl00::SceneManager* gl00::SceneManager::GetInstance()
{
    return &g_scene_manager;
}

void gl00::SceneManager::RequestNewScene(gl00::Scene *scene)
{
    scene_to_install_ = scene;
} 

gl00::Scene* gl00::SceneManager::GetScene()
{
    return current_scene_;
}

void gl00::SceneManager::InstallScene(gl00::Scene* new_scene)
{
    bool had_graphics = has_graphics_;
    if (has_graphics_)
        KillGraphics();

    if (current_scene_)
    {
        current_scene_->OnUnInstall();
        delete current_scene_;
        current_scene_ = std::nullptr_t{};
    }

    current_scene_ = new_scene;
    if (current_scene_)
        current_scene_->OnInstall();

    if (had_graphics)
        StartGraphics();
}

void gl00::SceneManager::StartGraphics()
{
    if (!has_graphics_)
    {
        has_graphics_ = true;
        if (current_scene_)
            current_scene_->OnStartGraphics();
    }
}

void gl00::SceneManager::KillGraphics() {
    if (has_graphics_) {
        has_graphics_ = false;
        if (current_scene_) {
            current_scene_->OnKillGraphics();
        }
    }
}

void gl00::SceneManager::Poke(int i)
{
    current_scene_->Poke(i);
}

void gl00::SceneManager::UpdateMouseMotion(float x, float y)
{
    current_scene_->OnMouseMotion(x, y);
}

void gl00::SceneManager::UpdateJoy(float x, float y)
{
    current_scene_->OnJoy(x, y);
}

void gl00::SceneManager::SetScreenSize(int width, int height)
{
    if (screen_width_ != width || screen_height_ != height)
    {
        screen_width_ = width;
        screen_height_ = height;

        if (current_scene_)
            current_scene_->OnScreenResize(width, height);
    }
}

float gl00::SceneManager::GetScreenAspect()
{
    return screen_width_ / (float)screen_height_;
}

void gl00::SceneManager::DoFrame()
{
    if (scene_to_install_)
    {
        InstallScene(scene_to_install_);
        scene_to_install_ = std::nullptr_t{};
    }

    if (has_graphics_ && current_scene_)
        current_scene_->DoFrame();
}
