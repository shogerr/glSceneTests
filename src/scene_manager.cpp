#include "scene_manager.hpp"
#include "scene.hpp"

static SceneManager _sceneManager;

SceneManager::SceneManager()
{
    mCurScene = NULL;
    mSceneToInstall = NULL;

    mScreenWidth = 512;
    mScreenHeight = 512;

    has_graphics_ = false;
}

SceneManager* SceneManager::GetInstance()
{
    return &_sceneManager;
}

void SceneManager::RequestNewScene(Scene *newScene)
{
    mSceneToInstall = newScene;
} 

Scene* SceneManager::GetScene()
{
    return mCurScene;
}

void SceneManager::InstallScene(Scene* newScene)
{
    bool had_graphics = has_graphics_;
    if (has_graphics_)
        KillGraphics();

    if (mCurScene)
    {
        mCurScene->OnUnInstall();
        delete mCurScene;
        mCurScene = NULL;
    }

    mCurScene = newScene;
    if (mCurScene)
        mCurScene->OnInstall();

    if (had_graphics)
        startGraphics();
}

void SceneManager::startGraphics()
{
    if (!has_graphics_)
    {
        LOGI("SceneManager: starting graphics.\n");
        has_graphics_ = true;
        if (mCurScene)
        {
            LOGI("Calling OnStartGraphics()\n");
            mCurScene->OnStartGraphics();
        }
    }
}

void SceneManager::KillGraphics() {
    if (has_graphics_) {
        LOGI("SceneManager: killing graphics.\n");
        has_graphics_ = false;
        if (mCurScene) {
            mCurScene->OnKillGraphics();
        }
    }
}

void SceneManager::Poke(unsigned int i)
{
    mCurScene->Poke(i);
}

void SceneManager::setScreenSize(int width, int height)
{
    if (mScreenWidth != width || mScreenHeight != height)
    {
        mScreenWidth = width;
        mScreenHeight = height;

        if (mCurScene)
            mCurScene->OnScreenResize(width, height);
    }
}

float SceneManager::getScreenAspect()
{
    return mScreenWidth / (float)mScreenHeight;
}

void SceneManager::DoFrame()
{
    if (mSceneToInstall)
    {
        LOGI("Scene to install.\n");
        InstallScene(mSceneToInstall);
        mSceneToInstall = NULL;
    }

    if (has_graphics_ && mCurScene)
        mCurScene->DoFrame();
}