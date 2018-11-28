#include "camera.hpp"

gl00::Camera::Camera() :
    projection_(glm::perspective(glm::radians(65.0f),1.0f,0.01f,100.0f)),
    view_(glm::lookAt(glm::vec3(0.0f,1.0f,-1.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))
{ }

void gl00::Camera::SetProjectionMatrix(float fovy, float aspect_ratio, float znear, float zfar)
{
    projection_ = glm::perspective(glm::radians(fovy), aspect_ratio, znear, zfar);
}
