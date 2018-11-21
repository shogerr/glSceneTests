#include "../engine_parts.hpp"
#include "../model_single.hpp"

typedef struct particle {
    glm::vec3 s;
    glm::vec3 v;

    float e;
} Particle;

class TextureScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();

    TextureScene();

    void SetTextureView(int scene_view);

private:
    uint64_t last_frame_ns_;
    ModelSingle* sphere_model_;
    Shader* shader_;

    GLuint emptytex_;
    GLuint scenetex_;

    float theta_;
    int dir_;

    int texture_scene_;

    Particle focus_;


    void Step();
};