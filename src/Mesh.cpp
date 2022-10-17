#include "../include/Mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices){
    this->vertices = vertices;
    this->indices = indices;
    this->material = Material();
    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material mat){
    this->vertices = vertices;
    this->indices = indices;
    this->material = mat;
    setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = Material();
    setupMesh();
}

void Mesh::addTransform(mat4 transform){
    modelTransforms.push_back(transform);
}

void Mesh::addTexture(Texture texture){
    textures.push_back(texture);
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
    //Tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    //Bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader, ModelType modelType, ShaderType shaderType){
    switch(shaderType){
        case BASIC: {
            switch(modelType){
                case BUNNY: {
                    material.diffuse = vec3(1.0f, 0.0f, 0.0f);
                    break;
                } case ARMADILLO: {
                    material.diffuse = vec3(0.0f, 1.0f, 0.0f);
                    break;
                } case DRAGON: {
                    material.diffuse = vec3(0.0f, 0.0f, 1.0f);
                    break; 
                } case DAVID: {
                    material.diffuse = vec3(1.0f, 1.0f, 0.0f);
                    break;
                } default: {
                    material.diffuse = vec3(0.0f, 0.0f, 0.0f);
                }
            }
            shader.setVec3("diffuse", material.diffuse);
            shader.setVec3("ambient", material.ambient);
            shader.setVec3("specular", material.specular);
            shader.setFloat("specularity", material.specularity);
            break;
        }case TEXTURE: {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);
            shader.setInt("diffuseTexture",0);
            break;
        }case CUBEMAP: {
            switch(modelType){
                case TREE: {
                    shader.setBool("isTree", true);
                    shader.setVec3("color", material.diffuse);
                    break;
                } case SKYBOX: {
                    shader.setBool("isTree", false);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0].id);
                    shader.setInt("cubeMap", 0);
                    break;
                }
            }
            break;
        }case NORMAL: {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);
            shader.setInt("diffuseTexture",0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[1].id);
            shader.setInt("normalTexture",1);
            break;
        }case DISP: {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);
            shader.setInt("diffuseMap",0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[1].id);
            shader.setInt("normalMap",1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textures[2].id);
            shader.setInt("depthMap",2);
            break;
        }case ENV: {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);
            shader.setInt("lakeTexture",0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[1].id);
            shader.setInt("pebbleTexture",1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textures[2].id);
            shader.setInt("cubeMap",2);
            break;
        }
    }

    glBindVertexArray(VAO);
    for(auto m : modelTransforms){
        if(shaderType == BASIC || shaderType == TEXTURE || shaderType == NORMAL || shaderType == DISP || shaderType == ENV || shaderType == DEBUG){
            shader.setMat3("normalViewMatrix", mat3(transpose(inverse(m))));
        }
        shader.setMat4("model", m);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}