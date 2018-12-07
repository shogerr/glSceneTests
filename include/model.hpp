#ifndef __MODEL_HPP
#define __MODEL_HPP

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <mesh.hpp>
 
GLuint TextureFromFile(const char* path, std::string directory);

class Shader;

namespace gl00
{
    class Model
    {
    public:
        Model(std::string path);
        Model(std::string path, unsigned int instance_count);
        void Draw(Shader* shader);

        void UpdateModel(glm::mat4* model);
        void SetInstanceCount(unsigned int i);

        // TODO: take mMeshes out of public space
        std::vector<gl00::Mesh> meshes_;

        ~Model();


    private:
        std::vector<gl00::Mesh::Texture> loaded_textures_;
        std::string model_directory_;

        unsigned int instance_count_;

        // Model matrix applied to all meshes held by model.
        glm::mat4* model_;

        void LoadModel(std::string& path);

        void ProcessNode(aiNode* node, const aiScene* scene);

        gl00::Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

        std::vector<gl00::Mesh::Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name);
    };
}

#endif // !__MODEL_HPP
