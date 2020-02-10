#pragma once

#include "common.hpp"
#include <functional>

#include <iostream>

namespace gl00
{
    class Camera
    {
    public:
        // TODO Add a comment
        template <typename T>
        class PositionShim : public glm::vec3
        {
        public:
            PositionShim() {}
            PositionShim(std::function<void (void)> f) : f_(f) {}
            void X(T x)
            {
                std::cout << "in shim." << std::endl;
                this->x = x;
                f_();
            }
        private:
            std::function<void (void)> f_;
        };

        Camera();
        Camera(glm::vec3 eye);
        Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up = (glm::vec3(0.0f,1.0f,0.0f)));

        void SetPerspective(float fovy, float aspect_ratio, float znear, float zfar);

        glm::mat4 const & GetProjection() { return projection_; }
        glm::mat4 const & GetView() { return view_; }

        void Target(const glm::vec3);
        void Position(const glm::vec3);
        void View(const glm::mat4 view) { view_ = view; }

        void Update();

        void Test() { std::cout << "Testing" << std::endl; } 
        glm::vec3 const & Position() const { return position_; }
        glm::vec3 const & Target() const { return target_; }

        // Controls
        void Translate(float x, glm::vec3 direction);

        glm::vec3 position_;
        glm::vec3 target_;

        PositionShim<float> p_;
    protected:
        glm::vec3 up_;
    private:
        glm::mat4 projection_;
        glm::mat4 view_;
    public:
    };
}
