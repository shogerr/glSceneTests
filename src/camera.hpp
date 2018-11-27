#ifndef __CAMERA_HPP
#define __CAMERA_HPP

#include "common.hpp"

class Camera
{
public:
    Camera();
    glm::mat4 projection_;
    glm::mat4 view_;

    void SetProjectionMatrix(float fovy, float aspect_ratio, float znear, float zfar);
};

#endif // !__CAMERA_HPP