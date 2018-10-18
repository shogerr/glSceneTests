#ifndef __MESH_HPP
#define __MESH_HPP

#include <vector>
#include "common.hpp"

class Mesh
{
public:
    struct Vertex
    {
        glm::vec4 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

//private:
//    GLuint mVAO, mVBO, mEBO;
};
#endif // !__MESH_HPP
