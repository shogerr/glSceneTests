#include "common.hpp"
#include "scene_manager.hpp"
#include "engine.hpp"

#include "scenes/shader_scene.hpp"

static Engine* _engine = NULL;

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

Engine::Engine () : first_frame_(true), has_globjects_(false)
{
    _engine = this;
}

Engine::~Engine()
{

}

Engine* Engine::GetInstance()
{
    return _engine;
}

void Engine::SetScreenDimensions(int width, int height)
{
    if (mWindowWidth != width || mWindowHeight != height)
    {
        SceneManager* mgr = SceneManager::GetInstance();
        mWindowWidth = width;
        mWindowHeight = height;
        mgr->setScreenSize(width, height);
    }
}

void Engine::ConfigureOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

bool Engine::preRender()
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

void Engine::DoFrame()
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

bool Engine::initGLObjects()
{
    if (!has_globjects_)
    {
        LOGI("Initializing GL objects...\n");
        SceneManager* mgr = SceneManager::GetInstance();
        mgr->startGraphics();
        _logOpenGlError(glGetError());
        has_globjects_ = true;
    }

    return true;
}
