#ifndef __MESH_HPP
#define __MESH_HPP

#include <vector>
#include <string>

#include <assimp/types.h>

#include <gl00/common.hpp>
#include <gl00/shader.hpp>

namespace gl00
{
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

        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, unsigned int num_instances);
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
        ~Mesh() = default;

        void Draw(Shader* shader);
        void UpdateModel(glm::mat4* model);

        std::vector<Mesh::Texture> GetTextures();

        GLuint Vbo() { return vbo_; };
        int IndicesCount() { return index_count_; }
        int VertexCount() { return vertex_count_; };

        GLuint vao_;

        unsigned int instance_count_;
    private:
        std::vector<Mesh::Texture> textures_;

        unsigned int vertex_count_;
        unsigned int index_count_;

        GLuint vbo_, ebo_;
        GLuint model_bo_;

        virtual void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    };
}

#endif // !__MESH_HPP
