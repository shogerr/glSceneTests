#include "texture_scene.hpp"
#include <chrono>
#include <random>

#define EPSILON 0.1f

TextureScene::TextureScene()
{
    focus_ = Particle{ glm::vec3(0.0f), glm::vec3(0.05f, 0.05f, .0f), 0.9f };
    texture_scene_ = 1;
}

void TextureScene::SetTextureView(int scene_view)
{
    texture_scene_ = scene_view;
}

bool Comparef(float a, float b)
{
    float c = a - b;
    if (c < EPSILON && -c < EPSILON)
        return true;
    return false;
}

void TextureScene::OnStartGraphics()
{
    int width = 1, height = 1;

    theta_ = 1.0f;
    dir_ = 1;

    last_frame_ns_ = 0;

    sphere_model_ = new ModelSingle("../src/test/face_textured.obj");

    std::vector<std::pair <GLenum, std::string >> shaderFilenames = { {GL_VERTEX_SHADER, "../src/shaders/texture_vertex.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/texture_fragment.glsl"} };

    shader_ = new Shader(shaderFilenames);


    glGenTextures(1, &emptytex_);

    glm::vec4 image = glm::vec4(0.0, .0, .2, 1.0);
    //unsigned char image[4] = { 255, 255, 255, 255 };

    glBindTexture(GL_TEXTURE_2D, emptytex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &image[0]);

    scenetex_ = ModelSingle::TextureFromFile("eyes.jpg", "../src/test");

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 2.0f);
}

void TextureScene::Step()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

    float dt = float(now - last_frame_ns_) * 0.000000001f;

    if (last_frame_ns_ <= 0 || dt > 1)
        goto update_time;

    if (theta_ - 8.0f < .1f && -1.0f*(theta_ - 8.0f) < .1f)
        dir_ = -1;
    else if (theta_ - -4.0f < .1f && -1.0f*(theta_ - -4.0f) < .1f)
        dir_ = 1;

    focus_.s += dt * focus_.v;

    if (Comparef(focus_.s.x, 0.0))
    {
        focus_.s.x = 0.1f;
        focus_.v.x = -focus_.e * focus_.v.x;
    }
    else if (Comparef(focus_.s.x, 1.0))
    {
        focus_.s.x = .9f;
        focus_.v.x = -focus_.e*focus_.v.x;
    }

    if (Comparef(focus_.s.y, 0.0))
    {
        focus_.s.y = 0.1f;
        focus_.v.y = -focus_.e * focus_.v.y;
    }
    else if (Comparef(focus_.s.y, 1.0))
    {
        focus_.s.y = .9f;
        focus_.v.y = -focus_.e * focus_.v.y;
    }

    //LOGI("particle (x, y), v_x, v_y: (%f, %f), %f, %f\n", focus_.s.x, focus_.s.y, focus_.v.x, focus_.v.y);

    theta_ += dt * dir_;

    glProgramUniform1f(shader_->mProgram, 0, theta_);
    glProgramUniform2f(shader_->mProgram, 1, focus_.s.x, focus_.s.y);
    glProgramUniform1i(shader_->mProgram, 3, texture_scene_);

update_time:
    last_frame_ns_ = now;
}

void TextureScene::DoFrame()
{
    glClearColor(.2f, .2f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_->mProgram);

    Step();

    //glUniform1i(shader_->mProgram, 2);

    switch (texture_scene_)
    {
    case 1:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, emptytex_);
        break;
    case 2:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sphere_model_->meshes_[0].textures_[0].id);
        break;
    case 4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 5:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case 6:
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, scenetex_);
        break;
    }

    glBindVertexArray(sphere_model_->meshes_[0].vao_);
    glDrawElements(GL_TRIANGLES, sphere_model_->meshes_[0].indices_.size(), GL_UNSIGNED_INT, 0);
}
