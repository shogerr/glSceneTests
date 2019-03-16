#ifndef __BEZIER_SCENE_HPP
#define __BEZIER_SCENE_HPP

#include <gl00/engine_parts.hpp>
#include "../gizmos/line.hpp"
#include <vector>
#include <bitset>

class BezierScene : public gl00::Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();
    virtual void OnKillGraphics();
    virtual void Poke(unsigned int e)
    {
        animation_state_ ^= 1ULL << e;
    }
private:
    GLuint linebo_;
    GLuint colorbo_;

    gl00::Shader* control_shader_;
    gl00::Shader* line_shader_;

    glm::mat4 projection_, view_;

    std::bitset<6> animation_state_;

    std::vector<gl00::Line> lines_;

    uint64_t lastframe_ns_;
    float timer_;
    float rot_timer_;

    glm::vec3 CasteljauPoint(int r, int i, double t, std::vector<glm::vec3>* points);
};

#endif // !__BEZIER_SCENE_HPP
