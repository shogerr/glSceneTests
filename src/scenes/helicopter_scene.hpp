#ifndef __HELICOPTER_SCENE_HPP
#define __HELICOPTER_SCENE_HPP

#include "engine_parts.hpp"
#include "model.hpp"

class HelicopterScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();

    void toggleView();

private:
    enum {VB_HELI, VB_HELI_MVP, VB_BLADES, VB_BLADE_NORMS, VB_BLADES_MVP, VB_CUBE, VB_CUBE_NORMS, VB_CUBE_MVP, VB_COUNT};
    enum {VA_HELI, VA_BLADES, VA_CUBE, VA_COUNT};
    enum {VIEW_OUTSIDE, VIEW_INSIDE};

    Shader* mShader;
    GLuint mVB[VB_COUNT];
    GLuint mVA[VA_COUNT];
    GLuint mEBO;

    glm::mat4 mView, mProjection;

    uint64_t mLastFrameNs;
    Model* mHelicopterMesh;
    
    float mAngularV[3];
    float mAngles[3] = { 0,0,0 };

    int mCurrentView;

    glm::vec3 mBladeNorms[6];

    void step();
    void updateProjectionMatrix();
};
#endif //__HELICOPTER_SCENE_HPP

