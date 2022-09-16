#ifndef MESH_HPP
#define MESH_HPP

#include "Shader.hpp"

struct Vertex {
    vec3 pos;
    vec3 normal;
    vec2 texCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
    public:
        vector<Vertex> vertices; 
        vector<unsigned int> indices;
        vector<Texture> textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void draw(Shader &shader);
    private:
        //Rendering Data
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};

#endif