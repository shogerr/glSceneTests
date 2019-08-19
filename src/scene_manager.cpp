#include <gl00/scene_manager.hpp>
#include <gl00/scene.hpp>

static gl00::SceneManager g_scene_manager{};

gl00::SceneManager::SceneManager()
{
    screen_width_ = 512;
    screen_height_ = 512;

    has_graphics_ = false;
}

gl00::SceneManager& gl00::SceneManager::GetInstance()
{
    static gl00::SceneManager scene_manager;
    return scene_manager;
}

void gl00::SceneManager::RequestNewScene(std::unique_ptr<gl00::Scene> scene)
{
    //scene_to_install_ = std::make_unique<gl00::Scene>(*scene);
    scene_to_install_ = std::move(scene);
} 

gl00::Scene* gl00::SceneManager::GetScene()
{
    return current_scene_.get();
}

void gl00::SceneManager::InstallScene(std::unique_ptr<gl00::Scene> new_scene)
{
    bool had_graphics = has_graphics_;
    if (has_graphics_)
        KillGraphics();

    if (current_scene_)
        current_scene_->OnUnInstall();

    current_scene_ = std::move(new_scene);

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

void gl00::SceneManager::Poke(unsigned int i)
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
        InstallScene(std::move(scene_to_install_));
    }

    if (has_graphics_ && current_scene_)
        current_scene_->DoFrame();
}
