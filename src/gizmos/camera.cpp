#include <gl00/camera.hpp>
#include <iostream>

gl00::Camera::Camera()
    : projection_(glm::perspective(glm::radians(45.0f), 1.0f, 1.f, 100.0f))
    , position_({0.f, 0.f, -2.f})
    , up_({0.f, 1.f, 0.f})
    , target_({0.f, 0.f, 0.f})
    , view_(glm::lookAt(position_, target_, up_))
{
    std::function<void (void)> f = [this]() { Update (); };
    p_ = f;
    //view_ = glm::lookAt(position_, glm::vec3(0.0f), up_);
    auto print_vec = [](glm::vec3 v) {
        std::cout << v.x << std::endl << v.y << std::endl << v.z << std::endl;
    };

    print_vec(Position());
}

gl00::Camera::Camera(glm::vec3 eye) : Camera()
{
    position_ = eye;
    Update();
}

gl00::Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : projection_(glm::perspective(glm::radians(45.f), 1.f, .01f, 100.f))
    , position_(position)
    , target_(target)
    , up_(up)
    , view_(glm::lookAt(position, target, up))
{ }

void gl00::Camera::SetPerspective(float fovy, float aspect_ratio, float znear, float zfar)
{
    projection_ = glm::perspective(glm::radians(fovy), aspect_ratio, znear, zfar);
}

void gl00::Camera::Update()
{
    view_ = glm::lookAt(position_, target_, up_);
}

void gl00::Camera::Target(const glm::vec3 target)
{
    target_ = target;
    Update();
}

void gl00::Camera::Position(const glm::vec3 position)
{
    position_ = position;
    Update();
}

void gl00::Camera::Translate(float x, glm::vec3 direction)
{
    position_ += x * direction;
    target_ += x * direction;
    Update();
}