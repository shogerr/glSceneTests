#ifndef __LINE_HPP
#define __LINE_HPP

namespace gl00
{
    class Line
    {
    public:
        Line() : resolution_(1000) {}

        void Clean()
        {
            LOGI("Cleaning up a Line.\n");
            glDisableVertexAttribArray(0);
            glDeleteVertexArrays(1, &vao_);
            glDeleteBuffers(1, &vbo_);
        }

        std::vector<glm::vec3> control_points_{ {0.f,0.f,0.f}, {0.f,1.f,0.f} };

        void Setup()
        {
            // Generates and binds values to the position vec3 at layout position 0.
            glGenBuffers(1, &vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, (resolution_ + control_points_.size()) * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);

            glGenVertexArrays(1, &vao_);
            glBindVertexArray(vao_);

            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);
        }

        int Size()
        {
            return resolution_ + static_cast<int>(control_points_.size());
        }

        void Update()
        {

            glBindBuffer(GL_ARRAY_BUFFER, vbo_);

            glm::vec3* points = (glm::vec3*)glMapBufferRange(GL_ARRAY_BUFFER, 0, Size() * sizeof(glm::vec3), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            for (double t = 0.; t <= 1.; t += .001)
            {
                *points = CasteljauPoint(control_points_.size() - 1, 0, t, control_points_);
                points++;
            }

            for (auto p : control_points_)
            {
                *points = p;
                points++;
            }

            glUnmapBuffer(GL_ARRAY_BUFFER);

        }

        void DrawControlPoints(gl00::Shader* shader)
        {

        }


        glm::vec3 CasteljauPoint(std::size_t right_end, std::size_t index, double t, std::vector<glm::vec3>& points)
        {
            if (right_end == 0)
                return points.at(index);

            glm::vec3 p1 = CasteljauPoint(right_end - 1, index, t, points);
            glm::vec3 p2 = CasteljauPoint(right_end - 1, index + 1, t, points);

            return glm::vec3((1 - t) * p1.x + t * p2.x, (1 - t) * p1.y + t * p2.y, (1 - t) * p1.z + t * p2.z);
        }

        GLuint GetVAO() { return vao_; }
        GLuint GetVBO() { return vbo_; }
        int GetResolution() { return resolution_; }

        GLuint vao_{ 0 };
        GLuint vbo_{ 0 };

        int resolution_;
    };

}

#endif // !__LINE_HPP