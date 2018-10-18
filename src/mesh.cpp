#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
    mVertices = vertices;
    mIndices = indices;
}
