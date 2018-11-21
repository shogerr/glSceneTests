#include "model_single.hpp"
#include <assimp/postprocess.h>
#include <SOIL2/SOIL2.h>
#include <fstream>

ModelSingle::ModelSingle(std::string path)
{
    LoadModel(path);
}

void ModelSingle::LoadModel(std::string& path)
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

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene)
    {
        printf("%s\n", importer.GetErrorString());
        return;
    }

    printf("%d\n", scene->mMeshes[0]->mNumVertices);

    model_directory_ = path.substr(0, path.find_last_of('/'));
    LOGI("model directory: %s\n", model_directory_.c_str());

    ProcessNode(scene->mRootNode, scene);
    //meshes_.push_back(ProcessMesh(scene->mMeshes[0], scene));
}

void ModelSingle::ProcessNode(aiNode * node, const aiScene * scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(ProcessMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; ++i)
        ProcessNode(node->mChildren[i], scene);
}

MeshSingle ModelSingle::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<MeshSingle::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<MeshSingle::Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        MeshSingle::Vertex vertex;
        vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->HasTextureCoords(0))
        {
            vertex.texcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
            vertex.texcoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    
    printf("num faces:%d\n", mesh->mNumFaces);
    // Setup indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process Materials 
    if (mesh->mMaterialIndex >= 0)
    {
        LOGI("Found materials\n");
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<MeshSingle::Texture> diffuse_maps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    }

    return MeshSingle(vertices, indices, textures);
}

std::vector<MeshSingle::Texture> ModelSingle::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<MeshSingle::Texture> textures;

    bool skip = false;

    LOGI("Texture count: %d\n", material->GetTextureCount(type));
    for (GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        LOGI("material: texture: %s\n", str.C_Str());
        for (GLuint j = 0; j < loaded_textures_.size(); ++j)
        {
            if (loaded_textures_[j].path == str)
            {
                textures.push_back(loaded_textures_[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            MeshSingle::Texture texture;
            texture.id = TextureFromFile(str.C_Str(), model_directory_);
            texture.type = type_name;
            texture.path = str;
            textures.push_back(texture);

            loaded_textures_.push_back(texture);
        }
    }

    return textures;
}

GLuint ModelSingle::TextureFromFile(const char* path, std::string directory)
{
    std::string filename = directory + '/' + std::string(path);
    //std::string filename = "./" + std::string(path);
    GLuint texture_id = 0;
    int width, height;
    
    std::ifstream fin(filename.c_str());
    if (!fin.fail())
        fin.close();
    else
    {
        printf("Couldn't open file: %s\n", filename.c_str());
        return texture_id;
    }

    glGenTextures(1, &texture_id);

    LOGI("filename: %s\n", filename.c_str());

    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_FRAGMENT_COLOR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_FRAGMENT_COLOR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 11);
    glBindTexture(GL_TEXTURE_2D, 0);

    LOGI("image loaded: %s\n", filename.c_str());
    SOIL_free_image_data(image);
    return texture_id;
}