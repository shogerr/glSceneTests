#include "scene_manager.hpp"
#include "scene.hpp"

static SceneManager _sceneManager;

SceneManager::SceneManager()
{
    mCurScene = NULL;
    mSceneToInstall = NULL;

    mScreenWidth = 512;
    mScreenHeight = 512;
}

SceneManager* SceneManager::getInstance()
{
    return &_sceneManager;
}

void SceneManager::RequestNewScene(Scene *newScene)
{
    mSceneToInstall = newScene;
} 

Scene* SceneManager::getScene()
{
    return mCurScene;
}

void SceneManager::installScene(Scene* newScene)
{
    if (mCurScene)
    {
        delete mCurScene;
        mCurScene = NULL;
    }

    mCurScene = newScene;
    startGraphics();
}

void SceneManager::startGraphics()
{
    if (mCurScene)
    {
        LOGI("Found scene\n");
        mCurScene->onStartGraphics();
    }
}

void SceneManager::setScreenSize(int width, int height)
{
    if (mScreenWidth != width || mScreenHeight != height)
    {
        mScreenWidth = width;
        mScreenHeight = height;

        if (mCurScene)
            mCurScene->onScreenResize(width, height);
    }
}

float SceneManager::getScreenAspect()
{
    return mScreenWidth / (float)mScreenHeight;
}

void SceneManager::doFrame()
{
    if (mSceneToInstall)
    {
        LOGI("Scene to install.\n");
        installScene(mSceneToInstall);
        mSceneToInstall = NULL;
    }

    mCurScene->doFrame();
}