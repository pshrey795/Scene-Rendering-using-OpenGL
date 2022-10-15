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
    NORMAL, 
    ENV,
    DISP,
    SHADOW,
    CUBE_MAP
};

struct Texture {
    unsigned int id;
    TextureType type;
    int texUnit; 
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    float specularity;
    Material(){
        ambient = vec3(0.1f, 0.1f, 0.1f);
        diffuse = vec3(0.0f, 0.0f, 0.0f);
        specular = vec3(0.8f, 0.8f, 0.8f);
        emission = vec3(0.0f, 0.0f, 0.0f);
        specularity = 10.0f;
    }
};

enum ModelType {
    NONE,
    GRASS, 
    ROAD, 
    LAKE, 
    SKYBOX,
    BUNNY,
    ARMADILLO,
    DRAGON,
    DAVID,
    TREE
};

enum ShaderType{
    BASIC,
    TEXTURE,
    CUBEMAP,
    LIGHT
};

void buildSquare(vector<Vertex> &vertices, vector<unsigned int> &indices);
mat4 getTransform(vec3 scale, vec3 rotateAxis, float rotateAngle, vec3 translate);

#endif