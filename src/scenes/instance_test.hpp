#ifndef __INSTANCE_TEST_HPP
#define __INSTANCE_TEST_HPP

#include "../engine_parts.hpp"

class InstanceTest : public Scene
{
public:
    virtual void DoFrame();
    virtual void OnStartGraphics();
private:
    Model* scene_model_;

    Shader* shader_;

    glm::vec3 view_position_;
    glm::mat4 projection_;
    glm::mat4 view_;
    
    uint64_t last_frame_ns_;

    void Step();
};

#endif // !__INSTANCE_TEST_HPP
