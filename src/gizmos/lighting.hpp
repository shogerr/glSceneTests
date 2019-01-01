#include <gl00/common.hpp>

namespace gl00
{
    class Lighting
    {
    public:
        typedef struct light {
            glm::vec3 ambient = glm::vec3(.1f);
            glm::vec3 diffuse = glm::vec3(1.0f);
            glm::vec3 specular = glm::vec3(1.0f);
        } Light;

        typedef struct pointLight {
            glm::vec3 position;

            float constant = 1.0f;
            float linear = .09f;
            float quadratic = .032f;

            Light light;
        } PointLight;

        typedef struct spotLight {
            glm::vec3 position = glm::vec3(0.f, 6.f, 0.f);
            glm::vec3 direction = glm::vec3(0.f, -1.f, 0.f);
            float cutoff = glm::cos(glm::radians(12.5f));
            float outer_cutoff = glm::cos(glm::radians(17.5f));

            float constant = 1.0f;
            float linear = .09f;
            float quadratic = .032f;

            Light light;
        } SpotLight;

        static void FillSpotLight(GLuint program, GLuint loc, const Lighting::SpotLight &spotlight)
        {
            glProgramUniform3fv(program, loc, 1, glm::value_ptr(spotlight.position));
            glProgramUniform3fv(program, loc + 1, 1, glm::value_ptr(spotlight.direction));
            glProgramUniform1f(program, loc + 2, spotlight.cutoff);
            glProgramUniform1f(program, loc + 3, spotlight.outer_cutoff);
            glProgramUniform1f(program, loc + 4, spotlight.constant);
            glProgramUniform1f(program, loc + 5, spotlight.linear);
            glProgramUniform1f(program, loc + 6, spotlight.quadratic);
            glProgramUniform3fv(program, loc + 7, 1, glm::value_ptr(spotlight.light.ambient));
            glProgramUniform3fv(program, loc + 8, 1, glm::value_ptr(spotlight.light.diffuse));
            glProgramUniform3fv(program, loc + 9, 1, glm::value_ptr(spotlight.light.specular));
        }

        static void FillPointLight(GLuint program, GLuint loc, const Lighting::PointLight &pointlight)
        {
            glProgramUniform3fv(program, loc, 1, glm::value_ptr(pointlight.position));
            glProgramUniform1f(program, loc + 1, pointlight.constant);
            glProgramUniform1f(program, loc + 2, pointlight.linear);
            glProgramUniform1f(program, loc + 3, pointlight.quadratic);
            glProgramUniform3fv(program, loc + 4, 1, glm::value_ptr(pointlight.light.ambient));
            glProgramUniform3fv(program, loc + 5, 1, glm::value_ptr(pointlight.light.diffuse));
            glProgramUniform3fv(program, loc + 6, 1, glm::value_ptr(pointlight.light.specular));
        }
    };
}