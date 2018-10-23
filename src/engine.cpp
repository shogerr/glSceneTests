#include "common.hpp"
#include "scene_manager.hpp"
#include "engine.hpp"

#include "scenes/texture_scene.hpp"

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

Engine::Engine()
{
    _engine = this;
}

Engine::~Engine()
{

}

Engine* Engine::getInstance()
{
    return _engine;
}

void Engine::cycle()
{
    doFrame();
}

void Engine::setScreenDimensions(int width, int height)
{
    if (mWindowWidth != width || mWindowHeight != height)
    {
        SceneManager* mgr = SceneManager::getInstance();
        mWindowWidth = width;
        mWindowHeight = height;
        mgr->setScreenSize(width, height);
    }
}

bool Engine::preRender()
{
    do
    {
        if (!mHasGLObjects)
        {
           if (!initGLObjects())
                return false;
        }
    } while (0);
    return true;
}

void Engine::doFrame()
{
    if (!preRender())
        return;

    SceneManager* mgr = SceneManager::getInstance();
    if (mIsFirstFrame)
    {
        mIsFirstFrame = false;
        mgr->RequestNewScene(new TextureScene());
    }

    mgr->doFrame();

    GLenum e;
    if ((e = glGetError()) != GL_NO_ERROR)
    {
        _logOpenGlError(e);
    }
}

bool Engine::initGLObjects()
{
    if (!mHasGLObjects)
    {
        LOGI("Initializing GL objects...\n");
        SceneManager* mgr = SceneManager::getInstance();
        mgr->startGraphics();
        _logOpenGlError(glGetError());
        mHasGLObjects = true;
    }

    return true;
}
