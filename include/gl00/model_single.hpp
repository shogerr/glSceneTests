#ifndef __MODEL_SINGLE_HPP
#define __MODEL_SINGLE_HPP

#include <string>

#include <gl00/common.hpp>
#include <gl00/shader.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "mesh_single.hpp"

namespace gl00
{
    class ModelSingle
    {
    public:
        ModelSingle(std::string path);
        void Draw(gl00::Shader* shader);

        // TODO: take mMeshes out of public space
        std::vector<MeshSingle> meshes_;

        static GLuint TextureFromFile(const char* path, std::string directory);
    private:
        std::vector<MeshSingle::Texture> loaded_textures_;
        std::string model_directory_;

        void LoadModel(std::string& path);

        void ProcessNode(aiNode* node, const aiScene* scene);
        MeshSingle ProcessMesh(aiMesh* mesh, const aiScene* scene);

        std::vector<MeshSingle::Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name);
    };
}

#endif // !__MODEL_HPP
