#ifndef MESH_HPP
#define MESH_HPP

#include "Shader.hpp"
#include "Shape.hpp"

class Mesh {
    public:
        vector<Vertex> vertices; 
        vector<unsigned int> indices;
        vector<Texture> textures;

        //We don't take textures from the constructor, we set them later using explicit setters 
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void draw(Shader &shader, ModelType modelType, ShaderType shaderType);

    private:
        //Rendering Data
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};

#endif