#ifndef __MODEL_HPP
#define __MODEL_HPP

#include <string>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <gl00/mesh.hpp>
#include <gl00/shader.hpp>
 

namespace gl00
{
    GLuint TextureFromFile(const char* path, std::string directory);

    //class Shader;

    class Model
    {
    public:
        Model(std::string path, size_t instance_count = 1);

        ~Model();

        void Draw(Shader& shader);

        void UpdateModel(glm::mat4 model);
        void UpdateModel(std::shared_ptr<glm::mat4[]> model);
        void UpdateModel(std::vector<glm::mat4> &model);

        void SetInstanceCount(size_t i);

        gl00::Mesh & GetMesh(size_t index);
    private:
        size_t instance_count_;

        std::vector<gl00::Mesh> meshes_;

        std::vector<gl00::Mesh::Texture> loaded_textures_;

        std::string model_directory_;

        // Model matrix applied to all meshes held by model.
        std::shared_ptr<glm::mat4[]> model_;

        void LoadModel(std::string& path);

        void ProcessNode(aiNode *node, const aiScene *scene);

        gl00::Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<gl00::Mesh::Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string type_name);
    };
}

#endif // !__MODEL_HPP
