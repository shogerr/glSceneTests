#include "common.hpp"
#include "scene_manager.hpp"
#include "engine.hpp"

#include "scenes/shader_scene.hpp"

static gl00::Engine* g_engine = NULL;

static const void _logOpenGlError(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR:
        LOGE("OpenGL Error: GL_NO_ERROR\n");
        break;

    default:
        LOGE("OpenGL Error: %#05x\n", err);
        break;
    }
}

gl00::Engine::Engine () : first_frame_(true), has_globjects_(false)
{
    g_engine = this;
}

gl00::Engine::~Engine()
{
    g_engine = NULL;
}

gl00::Engine* gl00::Engine::GetInstance()
{
    return g_engine;
}

void gl00::Engine::SetScreenDimensions(int width, int height)
{
    if (mWindowWidth != width || mWindowHeight != height)
    {
        SceneManager* mgr = SceneManager::GetInstance();
        mWindowWidth = width;
        mWindowHeight = height;
        mgr->SetScreenSize(width, height);
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
        SceneManager *mgr = SceneManager::GetInstance();
        mgr->KillGraphics();
        has_globjects_ = false;
    }

    LOGD("Engine: Context killed.");
}

bool gl00::Engine::preRender()
{
    do
    {
        if (!has_globjects_)
        {
           if (!initGLObjects())
                return false;
        }
    } while (0);

    ConfigureOpenGL();
    return true;
}

void gl00::Engine::DoFrame()
{
    if (!preRender())
        return;

    SceneManager* mgr = SceneManager::GetInstance();
    if (first_frame_)
    {
        first_frame_ = false;
        mgr->RequestNewScene(new ShaderScene());
    }

    mgr->DoFrame();

    GLenum e;
    if ((e = glGetError()) != GL_NO_ERROR)
    {
        _logOpenGlError(e);
    }
}

bool gl00::Engine::initGLObjects()
{
    if (!has_globjects_)
    {
        LOGI("Initializing GL objects...\n");
        SceneManager* mgr = SceneManager::GetInstance();
        mgr->StartGraphics();
        _logOpenGlError(glGetError());
        has_globjects_ = true;
    }

    return true;
}
