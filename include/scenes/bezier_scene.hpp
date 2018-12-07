#ifndef __BEZIER_SCENE
#define __BEZIER_SCENE

#include "engine_parts.hpp"
#include <vector>
#include <bitset>

class Line;

class BezierScene : public Scene
{
public:
    virtual void OnStartGraphics();
    virtual void DoFrame();
    virtual void OnKillGraphics();
    virtual void Poke(unsigned int e)
    {
        animation_state_ ^= 1ULL << e;
    }
private:
    GLuint linebo_;
    GLuint colorbo_;

    Shader* control_shader_;
    Shader* line_shader_;

    glm::mat4 projection_, view_;

    std::bitset<6> animation_state_;

    std::vector<Line> lines_;

    uint64_t lastframe_ns_;
    float timer_;
    float rot_timer_;

    glm::vec3 CasteljauPoint(int r, int i, double t, std::vector<glm::vec3>* points);
};

class Line
{
public:
    Line() : resolution_(1000) {}
    std::vector<glm::vec3> control_points_;

    void Setup()
    {
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
        return resolution_ + control_points_.size();
    }

    void UpdateLine()
    {

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);

        glm::vec3* points = (glm::vec3*)glMapBufferRange(GL_ARRAY_BUFFER, 0, Size() * sizeof(glm::vec3), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        for (double t = 0; t <= 1; t += .001)
        {
            *points = CasteljauPoint(control_points_.size() - 1, 0, t, &control_points_);
            points++;
        }

        for (auto p : control_points_)
        { 
            *points = p;
            points++;
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);

    }

    void DrawControlPoints(Shader* shader)
    {

    }


    glm::vec3 CasteljauPoint(int r, int i, double t, std::vector<glm::vec3>* points)
    {
        if (r == 0)
            return points->at(i);

        glm::vec3 p1 = CasteljauPoint(r - 1, i, t, points);
        glm::vec3 p2 = CasteljauPoint(r - 1, i+1, t, points);

        return glm::vec3((1-t) * p1.x + t * p2.x, (1-t) * p1.y + t * p2.y, (1-t) * p1.z + t * p2.z);
    }

    GLuint vao_;
    GLuint vbo_;

    int resolution_;
};

#endif // !__BEZIER_SCENE
