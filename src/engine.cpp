#include "common.hpp"
#include "scene_manager.hpp"
#include "test_scene.hpp"
#include "engine.hpp"

static Engine* _engine = NULL;

static const void _logOpenGlError(GLenum err)
{
    LOGE("%#05x\n", err);
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
        mgr->RequestNewScene(new TestScene());
    }

    mgr->doFrame();

    GLenum e;
    if ((e = glGetError()) != GL_NO_ERROR)
    {
        _logOpenGlError(glGetError());
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
