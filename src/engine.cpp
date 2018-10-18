#include "common.hpp"
#include "scene_manager.hpp"
#include "engine.hpp"

#include "helicopter_scene.hpp"

static Engine* _engine = NULL;

static const void _logOpenGlError(GLenum err)
{
    LOGE("OpenGL Error: %#05x\n", err);
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
            LOGI("Initializing GL Objects...");
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
        mgr->RequestNewScene(new HelicopterScene());
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
