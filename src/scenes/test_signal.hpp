#include <glad/glad.h>
#include <gl00/scene.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

namespace gl00
{
    namespace scenes
    {
        class TestSignal : public gl00::Scene
        {
        public:
            TestSignal();
            ~TestSignal();
            void OnStartGraphics();
            void DoFrame();
        private:
            std::vector<glm::vec3> points_;
            GLuint vao_;
            GLuint vbo_;
        };
    }
}
