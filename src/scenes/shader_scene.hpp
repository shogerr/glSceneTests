#include <gl00/engine_parts.hpp>
#include "../src/gizmos/lighting.hpp"
#include <bitset>

class ShaderScene : public gl00::Scene
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
    gl00::Shader* shader_;
    gl00::Model* scene_model_;

    glm::mat4 projection_, view_;
    glm::vec3 view_position_;

    uint64_t lastframe_;
    glm::vec2 time_;

    std::bitset<4> animation_state_;
    gl00::Lighting::SpotLight g_spotlight;
};