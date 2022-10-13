#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "common.hpp"

struct Vertex {
    vec3 pos;
    vec3 normal;
    vec2 texCoords;
    Vertex(){
        pos = vec3(0.0f, 0.0f, 0.0f);
        normal = vec3(0.0f, 0.0f, 0.0f);
        texCoords = vec2(0.0f, 0.0f);
    }
    Vertex(vec3 pos){
        this->pos = pos;
        this->normal = vec3(0.0f, 0.0f, 1.0f);
        this->texCoords = vec2(0.0f, 0.0f);
    }
    Vertex(vec3 pos, vec3 normal){
        this->pos = pos;
        this->normal = normal;
        this->texCoords = vec2(0.0f, 0.0f);
    }
    Vertex(vec3 pos, vec3 normal, vec2 texCoords){
        this->pos = pos;
        this->normal = normal;
        this->texCoords = texCoords;
    }
};

enum TextureType {
    DIFFUSE,
    SPECULAR, 
    LIGHT,
    NORMAL, 
    ENV,
    DISP,
    SHADOW
};

struct Texture {
    unsigned int id;
    TextureType type;
    int texUnit; 
};

enum ModelType {
    NONE,
    GRASS, 
    ROAD, 
    LAKE, 
    CUBEFACE,
    BUNNY,
    ARMADILLO,
    DRAGON,
    DAVID,
};

enum ShaderType{
    BASIC,
    TEXTURE,
    CUBEMAP
};

void buildSquare(vector<Vertex> &vertices, vector<unsigned int> &indices);
mat4 getTransform(vec3 scale, vec3 rotateAxis, float rotateAngle, vec3 translate);

#endif