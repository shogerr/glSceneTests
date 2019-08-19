#include "instance_test.hpp"

gl00::scenes::InstanceTest::InstanceTest() :
    view_position_({ 0.f,0.f,0.5f }),
    instance_count_(3),
    time_last_ns_(0),
    cube_model_("assets/cube_object.obj", instance_count_),
    sphere_model_("assets/sphere_object.obj", instance_count_)
{
    gl00::SceneManager& mgr = gl00::SceneManager::GetInstance();

    projection_ = glm::perspective(glm::radians(45.0f), mgr.GetScreenAspect(), 0.1f, 100.f);
    view_ = glm::lookAt(view_position_, glm::vec3(.15f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

gl00::scenes::InstanceTest::~InstanceTest()
{
    LOGD("Destroying InstanceTest.\n");
}

void gl00::scenes::InstanceTest::OnStartGraphics()
{
    using ShaderList = std::vector<std::pair <GLenum, std::string>>;
    using Vec3 = glm::vec3;

    ShaderList shader_files = { {GL_VERTEX_SHADER, "shaders/test.vs.glsl"},
                                {GL_FRAGMENT_SHADER, "shaders/test.fs.glsl"} };

    shader_ = std::make_unique<gl00::Shader>(shader_files);

    auto t = [](Vec3 x, Vec3 s = Vec3{ .1f }) {
        return glm::scale(glm::translate(glm::mat4(1.f), x), s);
    };

    std::vector<glm::mat4> p = { glm::scale(glm::mat4(1.f), Vec3{ .1f }) };

    for (int i = 1; i <= instance_count_; i++)
        p.push_back(t({i*.15f,0.f,0.f}));

    std::vector<glm::mat4> m(0);

    for (int i = 0; i < instance_count_; i++)
        m.push_back(t({ i * .15f,.1f,0.f }, Vec3{ .05f }));

    cube_model_.UpdateModel(p);
    sphere_model_.UpdateModel(m);

    time_last_ns_ = TimeNs();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

void gl00::scenes::InstanceTest::DoFrame()
{
    glClearColor(.5, .5, .5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_->program_);
    glProgramUniformMatrix4fv(shader_->program_, 0, 1, GL_FALSE, glm::value_ptr(view_));
    glProgramUniformMatrix4fv(shader_->program_, 1, 1, GL_FALSE, glm::value_ptr(projection_));

    cube_model_.Draw(*shader_.get());
    sphere_model_.Draw(*shader_.get());
}

void gl00::scenes::InstanceTest::OnKillGraphics() {}

void gl00::scenes::InstanceTest::OnUnInstall() {}
