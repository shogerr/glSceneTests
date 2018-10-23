#ifndef __MESH_HPP
#define __MESH_HPP

#include <vector>
#include <string>
#include "common.hpp"
#include <assimp\types.h>

class Mesh
{
public:
    struct Vertex
    {
        glm::vec4 position;
        glm::vec2 texcoords;
        glm::vec3 normal;
    };

    struct Texture
    {
        GLuint id;
        std::string type;
        aiString path;
    };

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    std::vector<Mesh::Texture> textures_;

    GLuint vao_, vbo_, ebo_;

private:
    virtual void SetupMesh();

};
#endif // !__MESH_HPP
