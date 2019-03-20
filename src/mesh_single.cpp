#include <gl00/mesh_single.hpp>
#include <gl00/shader.hpp>

gl00::MeshSingle::MeshSingle(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<MeshSingle::Texture>& textures)
{
    textures_ = textures;
    index_count_ = indices.size();
    vertex_count_ = vertices.size();
    SetupMesh(vertices, indices);
}

void gl00::MeshSingle::SetupMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    glGenBuffers(1, &ebo_);

    glBindBuffer(GL_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    glGenBuffers(1, &vbo_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


    printf("sizeof vertex: %d\n", sizeof(Vertex));
    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoords));

    glBindVertexArray(0);
}

void gl00::MeshSingle::Draw(gl00::Shader* shader)
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
    glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, 0);

}