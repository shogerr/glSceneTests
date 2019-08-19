#include <gl00/scene.hpp>
#include <vector>

namespace gl00
{
    namespace scenes
    {
        class Blank : public gl00::Scene
        {
        public:
            Blank();
            ~Blank();
            void OnStartGraphics();
        private:
        };
    }
}