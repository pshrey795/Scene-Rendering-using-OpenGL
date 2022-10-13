#include "../include/Mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices){
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::setupMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Setting up the vertex buffer
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //Setting up the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //Setting up the vertex attributes
    //Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //Texture Coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader, ShaderType shaderType){
    switch(shaderType){
        case BASIC: {
            shader.setVec3("color", vec3(1.0f, 1.0f, 1.0f));
            break;
        }case TEXTURE: {
            glActiveTexture(GL_TEXTURE0 + textures[0].texUnit);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);
            shader.setInt("tex0", textures[0].texUnit);
            break;
        }
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}