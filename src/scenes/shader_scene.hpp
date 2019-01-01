#include <gl00/engine_parts.hpp>
#include <bitset>

class Lighting
{
public:
    typedef struct light {
        glm::vec3 ambient = glm::vec3(1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
    } Light;

    typedef struct pointLight {
        glm::vec3 position;

        float constant = 1.0f;
        float linear = .09f;
        float quadratic = .32f;

        Light light;
    } PointLight;

    typedef struct spotLight {
        glm::vec3 position;
        glm::vec3 direction;
        float cutoff = glm::cos(glm::radians(12.5f));
        float outer_cutoff = glm::cos(glm::radians(15.0f));

        float constant = 1.0f;
        float linear = .09f;
        float quadratic = .32f;

        Light light;
    } SpotLight;

    static void FillSpotLight(GLuint program, GLuint loc, Lighting::SpotLight* spotlight)
    {
        glProgramUniform3fv(program, loc, 1, glm::value_ptr(spotlight->position));
        glProgramUniform3fv(program, loc + 1, 1, glm::value_ptr(spotlight->direction));
        glProgramUniform1f(program, loc + 2, spotlight->cutoff);
        glProgramUniform1f(program, loc + 3, spotlight->outer_cutoff);
        glProgramUniform1f(program, loc + 4, spotlight->constant);
        glProgramUniform1f(program, loc + 5, spotlight->linear);
        glProgramUniform1f(program, loc + 6, spotlight->quadratic);
        glProgramUniform3fv(program, loc + 7, 1, glm::value_ptr(spotlight->light.ambient));
        glProgramUniform3fv(program, loc + 8, 1, glm::value_ptr(spotlight->light.diffuse));
        glProgramUniform3fv(program, loc + 9, 1, glm::value_ptr(spotlight->light.specular));
    }

    static void FillPointLight(GLuint program, GLuint loc, Lighting::PointLight* pointlight)
    {
        glProgramUniform3fv(program, loc, 1, glm::value_ptr(pointlight->position));
        glProgramUniform1f(program, loc + 1, pointlight->constant);
        glProgramUniform1f(program, loc + 2, pointlight->linear);
        glProgramUniform1f(program, loc + 3, pointlight->quadratic);
        glProgramUniform3fv(program, loc + 4, 1, glm::value_ptr(pointlight->light.ambient));
        glProgramUniform3fv(program, loc + 5, 1, glm::value_ptr(pointlight->light.diffuse));
        glProgramUniform3fv(program, loc + 6, 1, glm::value_ptr(pointlight->light.specular));
    }
};

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
    Lighting::SpotLight g_spotlight;
};