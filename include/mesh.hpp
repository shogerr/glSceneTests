#ifndef __MESH_HPP
#define __MESH_HPP

#include <vector>
#include <string>
#include <common.hpp>
#include <assimp\types.h>
#include <shader.hpp>

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

        void Draw(Shader* shader);
        void UpdateModel(glm::mat4* model);

        ~Mesh();

        GLuint vao_;

    private:
        std::vector<Vertex> vertices_;
        std::vector<GLuint> indices_;
        std::vector<Mesh::Texture> textures_;

        unsigned int num_instances_;

        GLuint vbo_, ebo_;
        GLuint model_bo_;

        glm::mat4* model_;

        virtual void SetupMesh();
    };
}

#endif // !__MESH_HPP
