#include <gl00/common.hpp>
#include <gl00/engine.hpp>
#include <gl00/scene_manager.hpp>

#include "scenes/blank.hpp"

static const void _logOpenGlError(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:
        LOGE("OpenGL OK!\n");
        break;

    default:
        LOGE("OpenGL Error: %#05x\n", err);
        break;
    }
}

gl00::Engine::Engine() : first_frame_(true), has_globjects_(false), window_width_(512), window_height_(512)
{
    default_scene_ = std::make_unique<gl00::scenes::Blank>();
    LOGI("Engine on.\n");
}

gl00::Engine::~Engine()
{
}

gl00::Engine& gl00::Engine::GetInstance()
{
    static gl00::Engine instance;
    return instance;
}

void gl00::Engine::SetScreenDimensions(int width, int height)
{
    if (window_width_ != width || window_height_ != height)
    {
        SceneManager& mgr = SceneManager::GetInstance();
        window_width_ = width;
        window_height_ = height;
        mgr.SetScreenSize(width, height);
    }
}

void gl00::Engine::ConfigureOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void gl00::Engine::KillContext()
{
    KillGlObjects();
}

void gl00::Engine::KillGlObjects()
{
    if (has_globjects_)
    {
        SceneManager& mgr = SceneManager::GetInstance();
        mgr.KillGraphics();
        has_globjects_ = false;
    }

    LOGD("Engine: Context killed.");
}

bool gl00::Engine::PreRender()
{
    do
    {
        if (!has_globjects_)
        {
           if (!InitGLObjects())
                return false;
        }
    } while (0);

    ConfigureOpenGL();
    return true;
}

void gl00::Engine::DoFrame()
{
    if (!PreRender())
        return;

    SceneManager& mgr = SceneManager::GetInstance();

    if (first_frame_)
    {
        first_frame_ = false;

        if (default_scene_)
            mgr.RequestNewScene(std::move(default_scene_));
    }

    mgr.DoFrame();

    GLenum e;
    if ((e = glGetError()) != GL_NO_ERROR)
    {
        _logOpenGlError(e);
    }
}

bool gl00::Engine::InitGLObjects()
{
    if (!has_globjects_)
    {
        LOGI("Initializing GL objects...\n");
        SceneManager& mgr = SceneManager::GetInstance();
        mgr.StartGraphics();
        _logOpenGlError(glGetError());
        has_globjects_ = true;
    }

    return true;
}

void gl00::Engine::SetDefaultScene(std::unique_ptr<gl00::Scene> scene)
{
    default_scene_ = std::move(scene);
}
