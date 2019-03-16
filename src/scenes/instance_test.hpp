#ifndef __INSTANCE_TEST_HPP
#define __INSTANCE_TEST_HPP

#include <gl00/engine_parts.hpp>

namespace gl00::scenes
{
    class InstanceTest;
};

class gl00::scenes::InstanceTest : public gl00::Scene
{
public:
    virtual void DoFrame();
    virtual void OnStartGraphics();
    virtual void OnKillGraphics();
private:
    gl00::Shader* shader_;

    int instance_count_;

    gl00::Model* scene_model_;
    glm::mat4* p_model_;

    glm::vec3 view_position_;
    glm::mat4 projection_;
    glm::mat4 view_;

    uint64_t time_last_ns_;
};

/*
inline uint64_t TimeNs()
{
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}
*/
#endif // !__INSTANCE_TEST_HPP

