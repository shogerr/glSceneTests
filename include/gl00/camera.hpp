#pragma once

#include "common.hpp"

namespace gl00
{
    class Camera
    {
    public:
        Camera();
        Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up = glm::vec3(0.f,1.f,0.f));

        void SetProjectionMatrix(float fovy, float aspect_ratio, float znear, float zfar);

        glm::mat4 const& GetProjection() { return projection_; }
        glm::mat4 const& GetView() { return view_; }
    private:
        glm::mat4 projection_;
        glm::mat4 view_;

        glm::vec3 location_;
        glm::vec3 target_;
    };
}
