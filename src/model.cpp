#include <gl00/model.hpp>

#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>

gl00::Model::Model(std::string path, size_t instance_count)
{
    instance_count_ = instance_count;
    LoadModel(path);

    LOGI("Loaded model.\n");
    model_ = std::shared_ptr<glm::mat4[]>{ new glm::mat4[instance_count_] };
    for (int i = 0; i < instance_count_; i++)
        model_.get()[i] = glm::mat4(1.f);
}

gl00::Model::~Model()
{
    LOGD("Cleaning up model.\n");
    for (auto const &t : loaded_textures_)
        glDeleteTextures(1, &t.id);

    for (auto &m : meshes_)
        m.Clean();

}

void gl00::Model::Draw(Shader& shader)
{
    // Update and draw each mesh from the model.
    for (auto &m : meshes_)
    {
        m.UpdateModel(model_.get());
        m.Draw(shader);
    }
}

void gl00::Model::UpdateModel(std::shared_ptr<glm::mat4[]> model)
{
    //TODO fix this.
    model_ = model;
}

void gl00::Model::UpdateModel(glm::mat4 model)
{
    *model_.get() = model;
}

void gl00::Model::UpdateModel(std::vector<glm::mat4> &model)
{
    for (int i = 0; i < instance_count_; ++i)
        model_.get()[i] = model.at(i);
}

void gl00::Model::SetInstanceCount(size_t instance_count)
{
    instance_count_ = instance_count;
    for (auto &m : meshes_)
        m.instance_count_ = instance_count_;

}

gl00::Mesh& gl00::Model::GetMesh(size_t index)
{
    // TODO: insert return statement here
    return meshes_.at(index);
}

void gl00::Model::LoadModel(std::string& path)
{
    Assimp::Importer importer;

    std::ifstream fin(path.c_str());

    if (!fin.fail())
        fin.close();
    else
    {
        LOGE("Couldn't open file: %s\n", path.c_str());
        return;
    }

    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene)
    {
        LOGE("%s\n", importer.GetErrorString());
        return;
    }

    model_directory_ = path.substr(0, path.find_last_of('/'));

    LOGI("model directory: %s\n", model_directory_.c_str());

    ProcessNode(scene->mRootNode, scene);
}

void gl00::Model::ProcessNode(aiNode * node, const aiScene * scene)
{
    LOGI("Processing node\n");
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(ProcessMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
        ProcessNode(node->mChildren[i], scene);
}

gl00::Mesh gl00::Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<gl00::Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<gl00::Mesh::Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        gl00::Mesh::Vertex vertex;
        vertex.position = glm::vec4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->HasTextureCoords(0))
            vertex.texcoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex.texcoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    LOGI("Number of faces: %d\n", mesh->mNumFaces);

    // Setup indices
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process Materials 
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<gl00::Mesh::Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<gl00::Mesh::Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
        LOGI("Found materials:\nTextures: %zd\nSpecular Maps: %zd\n", diffuse_maps.size(), specular_maps.size());
    }

    return gl00::Mesh(vertices, indices, textures, instance_count_);
}

std::vector<gl00::Mesh::Texture> gl00::Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<gl00::Mesh::Texture> textures;

    bool skip = false;

    LOGI("Texture count: %d\n", material->GetTextureCount(type));
    for (GLuint i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        LOGI("material texture: %s\n", str.C_Str());
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
            gl00::Mesh::Texture texture;
            texture.id = TextureFromFile(str.C_Str(), model_directory_);
            texture.type = type_name;
            texture.path = str;
            textures.push_back(texture);

            loaded_textures_.push_back(texture);
        }
    }

    return textures;
}

GLuint gl00::TextureFromFile(const char* path, std::string directory)
{
    std::string filename = directory + '/' + std::string(path);

    GLuint texture_id = 0;
    int width, height, pixel_bytes;
    
    std::ifstream fin(filename.c_str());
    if (!fin.fail())
        fin.close();
    else
    {
        LOGE("Couldn't open file: %s\n", filename.c_str());
        return texture_id;
    }

    glGenTextures(1, &texture_id);

    LOGI("Filename: %s\n", filename.c_str());

    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &pixel_bytes, 4);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    //glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 11);
    glBindTexture(GL_TEXTURE_2D, 0);

    LOGI("Image loaded: %s\n", filename.c_str());

    stbi_image_free(image);

    return texture_id;
}
