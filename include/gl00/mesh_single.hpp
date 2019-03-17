#ifndef __MESH_SINGLE_HPP
#define __MESH_SINGLE_HPP

#include <vector>
#include <string>
#include "common.hpp"
#include <assimp\types.h>

class MeshSingle
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

    MeshSingle(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    std::vector<MeshSingle::Texture> textures_;

    GLuint vao_, vbo_, ebo_;

private:
    virtual void SetupMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

};
#endif // !__MESH_HPP
