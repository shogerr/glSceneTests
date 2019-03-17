// Based upon https://github.com/McNopper/OpenGL/tree/master/Example34

#include "scattering_scene.hpp"

const GLuint gl00::scenes::ScatteringScene::depth_pass_texture_size_ = 4096;

const glm::mat4 gl00::scenes::ScatteringScene::depth_pass_bias_matrix_ = { {.5,0.,0.,0.},{0.,.5,0.,0.},{0.,0.,.5,0.},{.5,.5,.5,1.} };

void gl00::scenes::ScatteringScene::OnStartGraphics()
{
    scene_model_ = new gl00::Model("assets/pigY1.obj");
    std::vector<std::pair <GLenum, std::string >> shader_filenames = { {GL_VERTEX_SHADER, "shaders/scattering.vs.glsl"},
                                                                       {GL_FRAGMENT_SHADER, "shaders/scattering.fs.glsl"} };

    std::vector<std::pair <GLenum, std::string >> depth_buffer_shader_filenames = { {GL_VERTEX_SHADER, "shaders/depthmap.vs.glsl"},
                                                                                    {GL_FRAGMENT_SHADER, "shaders/depthmap.fs.glsl"} };
    shaders_ = new gl00::Shader(shader_filenames);
    depth_buffer_shader_ = new gl00::Shader(depth_buffer_shader_filenames);

    light_pos_ = glm::vec3(2., 2., 3.);
    camera_.view_ = glm::lookAt(glm::vec3(3., 0., 3.), glm::vec3(0.), glm::vec3(0., 1., 0.));

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

    glClearDepth(1.);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    ////

    glUseProgram(shaders_->program_);
    glm::vec3 light_direction = glm::normalize(light_pos_);

    //Which way to multiply?
    light_direction = camera_.view_ * glm::vec4(light_direction, 1.);

    ////

    glUniform3fv(3, 1, glm::value_ptr(light_direction));
    // Set depth_pass_texture_ to 0.
    glUniform1i(4, 0);

    glUseProgram(depth_buffer_shader_->program_);
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, scene_model_->meshes_[0].Vbo());
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    /*
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 5.0f);
    */
    glUseProgram(0);

    time_last_ns_ = TimeNs();

    scene_model_->UpdateModel(&rotation_);

    counter_ = 0;
    Reshape();
}

void gl00::scenes::ScatteringScene::DoFrame()
{
    float now = TimeNs();
    float dt = float(now - time_last_ns_) * 0.000000001f;
    counter_ += dt;

    glm::mat4 view_matrix;
    glm::mat4 depth_pass_matrix;

    // Setup for depth pass texture.
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, depth_pass_texture_size_, depth_pass_texture_size_);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    theta_ += dt;
    //light_pos_ = glm::rotate(glm::mat4(1.), glm::two_pi<float>() * counter_ * .001f, glm::vec3(1., 0., 0.)) * glm::vec4(light_pos_,1.);
    view_matrix = glm::lookAt(light_pos_, glm::vec3(0.), glm::vec3(0.,1.,0.));

    rotation_ = glm::scale(glm::mat4(1.), glm::vec3(1.1));
    rotation_ = glm::rotate(rotation_, glm::two_pi<float>() * counter_ * .01f, glm::vec3(0., 1., 0.));

    depth_pass_matrix = depth_pass_matrix_ * view_matrix;

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(depth_buffer_shader_->program_);

    view_matrix *= rotation_;

    glProgramUniformMatrix4fv(depth_buffer_shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_matrix));
    //glProgramUniformMatrix4fv(depth_buffer_shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(camera_.projection_));
    glBindVertexArray(vao_);

    //glDrawElementsInstanced(GL_TRIANGLES, scene_model_->meshes_[0].IndicesCount(), GL_UNSIGNED_INT, 0, 1);
    glDrawArrays(GL_TRIANGLES, 0, scene_model_->meshes_[0].VertexCount());
    //scene_model_->Draw(depth_buffer_shader_);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Setup scene for normal rendering.
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    // Reset viewport
    glViewport(0, 0, 768, 768);
    glBindTexture(GL_TEXTURE_2D, depth_pass_texture_);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Switch to scene shaders
    glUseProgram(shaders_->program_);

    glm::vec3 light_direction = glm::normalize(light_pos_);

    //Which way to multiply?
    light_direction = camera_.view_ * glm::vec4(light_direction, 1.);

    ////

    glUniform3fv(3, 1, glm::value_ptr(light_direction));
    glProgramUniformMatrix4fv(shaders_->program_, 0, 1, GL_FALSE, glm::value_ptr(camera_.view_));
    glProgramUniformMatrix4fv(shaders_->program_, 2, 1, GL_FALSE, glm::value_ptr(depth_pass_matrix));

    scene_model_->Draw(shaders_);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    time_last_ns_ = now;
}

void gl00::scenes::ScatteringScene::Reshape()
{
    glm::mat4 projection;

    glUseProgram(depth_buffer_shader_->program_);

    projection = glm::perspective(45., 1., 1., 100.);

    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(projection));

    depth_pass_matrix_ = projection * depth_pass_bias_matrix_ * glm::mat4(1.);

    
    glUseProgram(shaders_->program_);
    glProgramUniformMatrix4fv(shaders_->program_, 1, 1, GL_FALSE, glm::value_ptr(camera_.projection_));
}
