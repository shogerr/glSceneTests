#ifndef __MODEL_HPP
#define __MODEL_HPP

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.hpp"
 
GLuint TextureFromFile(const char* path, std::string directory);

class Model
{
public:
    Model(std::string path);
    // TODO: take mMeshes out of public space
    std::vector<Mesh> meshes_;
private:
    std::vector<Mesh::Texture> loaded_textures_;
    std::string model_directory_;

    void loadModel(std::string& path);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name);
};

#endif // !__MODEL_HPP
