#ifndef __MODEL_HPP
#define __MODEL_HPP

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.hpp"
 
class Model
{
public:
    Model(std::string path);
    // TODO: take mMeshes out of public space
    std::vector<Mesh> mMeshes;
private:
    void loadModel(std::string& path);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
#endif // !__MODEL_HPP
