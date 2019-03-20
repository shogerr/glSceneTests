// Subsurface Scattering Scene
// Based upon https://github.com/McNopper/OpenGL/tree/master/Example34

#include <memory>

#include "scattering_scene.hpp"
#include "../key_codes.hpp"

#ifdef HEADER_OBJ_LOADER
#include "../OBJ_loader.h"
#endif

const GLuint gl00::scenes::ScatteringScene::depth_pass_texture_size_ = 4096;

const glm::mat4 gl00::scenes::ScatteringScene::depth_pass_bias_matrix_ = { {.5f,0.f,0.f,0.f},{0.f,.5f,0.f,0.f},{0.f,0.f,.5f,0.f},{.5f,.5f,.5f,1.f} };

void gl00::scenes::ScatteringScene::OnStartGraphics()
{
#ifndef HEADER_OBJ_LOADER
    scene_model_ = new gl00::ModelSingle("assets/pigY1.obj");
#else
    LOGI("HEADER_OBJ_LOADER defined in scattering scene.\n");
    std::unique_ptr<objl::Loader> Loader(new  objl::Loader);

#endif // !HEADER_OBJECT_LOADER

    std::vector<std::pair <GLenum, std::string >> shader_filenames = { {GL_VERTEX_SHADER, "shaders/scattering.vs.glsl"},
                                                                       {GL_FRAGMENT_SHADER, "shaders/scattering.fs.glsl"} };

    std::vector<std::pair <GLenum, std::string >> depth_buffer_shader_filenames = { {GL_VERTEX_SHADER, "shaders/depthmap.vs.glsl"},
                                                                                    {GL_FRAGMENT_SHADER, "shaders/depthmap.fs.glsl"} };
    shaders_ = new gl00::Shader(shader_filenames);
    depth_buffer_shader_ = new gl00::Shader(depth_buffer_shader_filenames);

    light_pos_ = glm::vec3(2.0f, 2.0f, 3.0f);

    fovy_ = glm::radians(40.0f);
    pause_ = false;
    camera_.projection_ = glm::perspective(fovy_, (GLfloat) width_ / height_, 1.0f, 100.0f);
    camera_.view_ = glm::lookAt(glm::vec3(0.f, 0.f, 3.5f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));

    glGenTextures(1, &depth_pass_texture_);
    glBindTexture(GL_TEXTURE_2D, depth_pass_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, depth_pass_texture_size_, depth_pass_texture_size_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_pass_texture_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOGE("GL Error 0x%x\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        exit(-1337);
    }

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef HEADER_OBJ_LOADER
    bool loadout = Loader->LoadFile("assets/pigY1.obj");
    objl::Mesh mesh = Loader->LoadedMeshes.front();

    vertex_count_ = mesh.Vertices.size();

    std::vector<objl::Vector3> vertices;
    std::vector<objl::Vector3> normals;

    for (auto v : mesh.Vertices)
    {
        vertices.push_back(v.Position);
        normals.push_back(v.Normal);
    }

    glGenBuffers(1, &dragon_vert_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, dragon_vert_vbo_);
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(objl::Vector3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &dragon_norm_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, dragon_norm_vbo_);
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(objl::Vector3), &normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    Loader.reset();
#endif
    
    glUseProgram(shaders_->program_);
    glm::vec3 light_direction = glm::normalize(light_pos_);

    //Which way to multiply?
    //light_direction = glm::vec3(camera_.view_ * glm::vec4(light_direction, 1.0f));
    light_direction = glm::mat3(camera_.view_) * light_direction;

    GLint normal_matrix_l = glGetUniformLocation(shaders_->program_, "u_normal_matrix");
    GLint depth_matrix_l = glGetUniformLocation(shaders_->program_, "depth_pass_matrix");
    LOGI("normal_matrix loc: %d\ndepth_matrix loc: %d\n", normal_matrix_l, depth_matrix_l);

    // Set uniform for light direction.
    glUniform3fv(5, 1, glm::value_ptr(light_direction));

    // Set depth_pass_texture_ to 0.
    glUniform1i(6, 0);


#ifdef HEADER_OBJ_LOADER
    // Set Vertex Attribute Object (VAO) for mesh display.
    glGenVertexArrays(1, &dragon_vao_);
    glBindVertexArray(dragon_vao_);
    glBindBuffer(GL_ARRAY_BUFFER, dragon_vert_vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, dragon_norm_vbo_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
#endif

    // Setup depth pass VAO
    glUseProgram(depth_buffer_shader_->program_);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);


#ifndef HEADER_OBJ_LOADER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene_model_->meshes_[0].Ebo());
    glBindBuffer(GL_ARRAY_BUFFER, scene_model_->meshes_[0].Vbo());

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 36, (GLvoid*)offsetof(gl00::MeshSingle::Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (GLvoid*)offsetof(gl00::MeshSingle::Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 36, (GLvoid*)offsetof(gl00::MeshSingle::Vertex, texcoords));
    glEnableVertexAttribArray(2);
#else
    glBindBuffer(GL_ARRAY_BUFFER, dragon_vert_vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
#endif

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    time_last_ns_ = TimeNs();

    counter_ = 0;

    //int width = SceneManager::GetInstance()->Width();
    //int height = SceneManager::GetInstance()->Height();
    OnScreenResize(width_, height_);
}

void gl00::scenes::ScatteringScene::DoFrame()
{
    uint64_t now = TimeNs();
    float dt = float(now - time_last_ns_) * 0.000000001f;

    if (!pause_)
        counter_ += dt * 5.0f;

    glm::mat4 view_matrix;
    glm::mat4 depth_pass_matrix;
    glm::mat4 model_matrix;
    glm::mat4 modelview_matrix;

    // Setup for depth pass texture.
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, depth_pass_texture_size_, depth_pass_texture_size_);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    view_matrix = glm::lookAt(light_pos_, glm::vec3(0.0f), glm::vec3(0.0f,1.0f,0.0f));

    depth_pass_matrix = depth_pass_matrix_ * view_matrix;

    glClear(GL_DEPTH_BUFFER_BIT);

    rotation_ = glm::rotate(glm::mat4(1.0f), glm::two_pi<float>() * counter_ * .01f, glm::vec3(0.f, 1.f, 0.f));

    glUseProgram(depth_buffer_shader_->program_);

    model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.05f));
    modelview_matrix = view_matrix * rotation_ * model_matrix;
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(modelview_matrix));

    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLES, 0, vertex_count_);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup scene for normal rendering.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // Reset viewport
    glViewport(0, 0, width_, height_);
    glBindTexture(GL_TEXTURE_2D, depth_pass_texture_);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Switch to scene shaders
    glUseProgram(shaders_->program_);

    // Set model matrix uniform.
    model_matrix = glm::mat4(1.0f);
    glm::mat3 normal_matrix = glm::mat3(camera_.view_ * rotation_ * model_matrix);

    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera_.view_));
    glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(rotation_ * model_matrix));
    glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(depth_pass_matrix));
    glUniformMatrix3fv(4, 1, GL_FALSE, glm::value_ptr(normal_matrix));

    glUniform1f(7, uniform_wrap_);
    glUniform1f(8, uniform_width_);
    glUniform1f(9, uniform_falloff_);

#ifndef HEADER_OBJ_LOADER
    scene_model_->Draw(shaders_);
#else
    glBindVertexArray(dragon_vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count_);
#endif

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    time_last_ns_ = now;
}

void gl00::scenes::ScatteringScene::OnScreenResize(int width, int height)
{
    width_ = width;
    height_ = height;

    glm::mat4 projection;

    glUseProgram(depth_buffer_shader_->program_);

    projection = glm::perspective(fovy_, (GLfloat)depth_pass_texture_size_/ (GLfloat)depth_pass_texture_size_, 1.0f, 100.f);

    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(projection));

    depth_pass_matrix_ = glm::mat4(1.0f);
    depth_pass_matrix_ *= depth_pass_bias_matrix_;
    depth_pass_matrix_ *= projection;
    
    glUseProgram(shaders_->program_);

    glProgramUniformMatrix4fv(shaders_->program_, 1, 1, GL_FALSE, glm::value_ptr(camera_.projection_));
}

void gl00::scenes::ScatteringScene::Poke(int i)
{
    switch (i)
    {
    case GL0K_q:
        uniform_wrap_ += .01f;
        break;
    case GL0K_a:
        uniform_wrap_ -= .01f;
        break;
    case GL0K_w:
        uniform_width_ += .01f;
        break;
    case GL0K_s:
        uniform_width_ -= .01f;
        break;
    case GL0K_e:
        uniform_falloff_ += .1f;
        break;
    case GL0K_d:
        uniform_falloff_ -= .1f;
        break;
    case GL0K_r:
        uniform_wrap_ = .1f;
        uniform_width_ = .5f;
        uniform_falloff_ = 5.f;
        break;
    case GL0K_p:
        pause_ = !pause_;
    }

    LOGI("wrap: %f, width: %f, falloff: %f\n", uniform_wrap_, uniform_width_, uniform_falloff_);
}
