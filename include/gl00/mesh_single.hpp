#ifndef __MESH_SINGLE_HPP
#define __MESH_SINGLE_HPP

#include <vector>
#include <string>
#include <assimp\types.h>
#include "common.hpp"
#include "shader.hpp"

namespace gl00
{
    class MeshSingle
    {
    public:
        struct Vertex
        {
            glm::vec4 position;
            glm::vec3 normal;
            glm::vec2 texcoords;
        };

        struct Texture
        {
            GLuint id;
            std::string type;
            aiString path;
        };

        MeshSingle(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

        void Draw(gl00::Shader* shader);

        GLuint Vbo() { return vbo_; }
        GLuint Ebo() { return ebo_; }

        GLsizei VertexCount() { return vertex_count_; }
        GLsizei IndexCount() { return index_count_; }

        std::vector<MeshSingle::Texture> textures_;
    private:
        void SetupMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

        unsigned int index_count_;
        unsigned int vertex_count_;

        GLuint vao_, vbo_, ebo_;
    };
}
#endif // !__MESH_HPP
