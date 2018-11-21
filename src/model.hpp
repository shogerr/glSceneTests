#ifndef __MODEL_HPP
#define __MODEL_HPP

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include "mesh.hpp"
#include "shader.hpp"
 
GLuint TextureFromFile(const char* path, std::string directory);

class Model
{
public:
    Model(std::string path);
    Model(std::string path, unsigned int instance_count);
    void Draw(Shader* shader);

    void UpdateModel(glm::mat4* model);

    // TODO: take mMeshes out of public space
    std::vector<Mesh> meshes_;

private:
    std::vector<Mesh::Texture> loaded_textures_;
    std::string model_directory_;

    unsigned int instance_count_;

    // Model matrix applied to all meshes held by model.
    glm::mat4* model_;
    glm::mat4 identity_;

    void LoadModel(std::string& path);

    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Mesh::Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name);
};

#endif // !__MODEL_HPP
