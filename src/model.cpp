#include "model.hpp"
#include <assimp/postprocess.h>
#include <fstream>

Model::Model(std::string path)
{
    loadModel(path);
}

void Model::loadModel(std::string& path)
{
    Assimp::Importer importer;

    std::ifstream fin(path.c_str());

    if (!fin.fail())
        fin.close();
    else
    {
        printf("Couldn't open file: %s\n", path.c_str());
        return;
    }

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene)
    {
        printf("%s\n", importer.GetErrorString());
        return;
    }

    printf("%d\n", scene->mMeshes[0]->mNumVertices);

    mMeshes.push_back(processMesh(scene->mMeshes[0], scene));
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;

    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;
        vertex.Position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        vertices.push_back(vertex);
    }
    
    printf("num faces:%d\n", mesh->mNumFaces);
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices, indices);
}