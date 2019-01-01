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
        ~Mesh();

        void Draw(Shader* shader);
        void UpdateModel(glm::mat4* model);

        std::vector<Mesh::Texture> GetTextures();
        int IndicesCount();

        GLuint vao_;

        unsigned int instance_count_;
    private:
        std::vector<Vertex> vertices_;
        std::vector<GLuint> indices_;
        std::vector<Mesh::Texture> textures_;


        GLuint vbo_, ebo_;
        GLuint model_bo_;

        virtual void SetupMesh();
    };
}

#endif // !__MESH_HPP
