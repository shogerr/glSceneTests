#include <gl00/mesh.hpp>

gl00::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Mesh::Texture> textures, unsigned int num_instances)
{
    vertices_ = vertices;
    indices_ = indices;
    textures_ = textures;

    instance_count_ = num_instances;
    SetupMesh();
}

gl00::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Mesh::Texture> textures) :
    Mesh(vertices, indices, textures, 1)
{}

gl00::Mesh::~Mesh()
{}

std::vector<gl00::Mesh::Texture> gl00::Mesh::GetTextures()
{
    return textures_;
}

int gl00::Mesh::IndicesCount()
{
    return indices_.size();
}

void gl00::Mesh::Draw(gl00::Shader* shader)
{
    for (GLuint i = 0; i < textures_.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        if (i%2 == 0)
            glProgramUniform1i(shader->program_, 4, i);
        else
            glProgramUniform1i(shader->program_, 5, i);

        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }
    glBindVertexArray(vao_);
    glDrawElementsInstanced(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0, instance_count_);
}

void gl00::Mesh::UpdateModel(glm::mat4* model)
{
    glBindBuffer(GL_ARRAY_BUFFER, model_bo_);
    glm::mat4* mesh_model_mat = (glm::mat4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, instance_count_ * sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    for (unsigned int i = 0; i < instance_count_; i++)
    {
        mesh_model_mat[i] = model[i];
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void gl00::Mesh::SetupMesh()
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    glGenBuffers(1, &model_bo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, model_bo_);
    glBufferData(GL_ARRAY_BUFFER, instance_count_ * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), &indices_[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Vertex Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, model_bo_);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(3 + i);
        glVertexAttribDivisor(3 + i, 1);
    }

    glBindVertexArray(0);
}
