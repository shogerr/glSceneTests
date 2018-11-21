#ifndef __TEST_SCENE_HPP
#define __TEST_SCENE_HPP

#include "../engine_parts.hpp"

const int MAX_INSTANCES = 32;

class TestScene : public Scene
{
public:
    TestScene();
    virtual void onStartGraphics();
    virtual void onScreenResize(int width, int height);
    virtual void DoFrame();

    void setViewScale(float s) { mViewScale += s; };
    void setViewRotate(float a, glm::vec3 v);

protected:
    void updateProjectionMatrix();
    void updateProjectionMatrix(float a);
private:
    enum {VB_INSTANCE, VB_MVP, VB_COUNT};

    glm::mat4 mView, mProjection;

    int mNumInstances;
   
    float mTimeCount;

    float mViewScale = 1;
    float mViewRotate;

    uint64_t mLastFrameNs;

    Shader* mShader;
    GLuint mVB[VB_COUNT];
    GLuint mVAO;

    void step();
};

#endif // __TEST_SCENE_HPP
