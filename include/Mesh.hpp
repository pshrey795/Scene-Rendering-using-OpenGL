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
        void draw(Shader &shader);

    private:
        //Rendering Data
        unsigned int VAO, VBO, EBO;
        void setupMesh();

        //Extracting a new texture from an image file 
        unsigned int getTextureFromFile(string fileName);
};

#endif