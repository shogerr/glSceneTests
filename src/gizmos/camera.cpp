#include <gl00/camera.hpp>

gl00::Camera::Camera() :
    projection_(glm::perspective(glm::radians(45.0f),1.0f,1.f,100.0f)),
    view_(glm::lookAt(glm::vec3(0.0f,1.0f,-1.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
{ }

gl00::Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up) :
    projection_(glm::perspective(glm::radians(45.f), 1.f, .01f, 100.f)),
    view_(glm::lookAt(eye, center, up))
{ }

void gl00::Camera::SetProjectionMatrix(float fovy, float aspect_ratio, float znear, float zfar)
{
    projection_ = glm::perspective(glm::radians(fovy), aspect_ratio, znear, zfar);
}
