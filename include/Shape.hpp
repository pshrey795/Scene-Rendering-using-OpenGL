#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "common.hpp"

struct Vertex {
    vec3 pos;
    vec3 normal;
    vec2 texCoords;
    vec3 tangent;
    vec3 bitangent;
    Vertex(){
        pos = vec3(0.0f, 0.0f, 0.0f);
        normal = vec3(0.0f, 0.0f, 0.0f);
        texCoords = vec2(0.0f, 0.0f);
        tangent = vec3(0.0f, 0.0f, 0.0f);
        bitangent = vec3(0.0f, 0.0f, 0.0f);
    }
    Vertex(vec3 pos){
        this->pos = pos;
        this->normal = vec3(0.0f, 0.0f, 1.0f);
        this->texCoords = vec2(0.0f, 0.0f);
        this->tangent = vec3(0.0f, 0.0f, 0.0f);
        this->bitangent = vec3(0.0f, 0.0f, 0.0f);
    }
    Vertex(vec3 pos, vec3 normal){
        this->pos = pos;
        this->normal = normal;
        this->texCoords = vec2(0.0f, 0.0f);
        this->tangent = vec3(0.0f, 0.0f, 0.0f);
        this->bitangent = vec3(0.0f, 0.0f, 0.0f);
    }
    Vertex(vec3 pos, vec3 normal, vec2 texCoords){
        this->pos = pos;
        this->normal = normal;
        this->texCoords = texCoords;
        this->tangent = vec3(0.0f, 0.0f, 0.0f);
        this->bitangent = vec3(0.0f, 0.0f, 0.0f);
    }
    Vertex(vec3 pos, vec3 normal, vec2 texCoords, vec3 tangent, vec3 bitangent){
        this->pos = pos;
        this->normal = normal;
        this->texCoords = texCoords;
        this->tangent = tangent;
        this->bitangent = bitangent;
    }
};

enum TextureType {
    DIFFUSE,
    SPECULAR, 
    CUBE_MAP,
    NORMAL_MAP,
    DEPTH_MAP,
    ENV_MAP
};

struct Texture {
    unsigned int id;
    TextureType type;
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
    TEST,
    GRASS, 
    ROAD, 
    LAKE, 
    SKYBOX,
    BUNNY,
    ARMADILLO,
    DRAGON,
    DAVID,
    TREE,
    SHADOW_MODEL,
    BASE
};

enum ShaderType{
    DEBUG,
    BASIC,
    TEXTURE,
    CUBEMAP,
    LIGHT,
    SHADOW,
    NORMAL,
    DISP,
    ENV
};

void buildSquare(vector<Vertex> &vertices, vector<unsigned int> &indices);
mat4 getTransform(vec3 scale, vec3 rotateAxis, float rotateAngle, vec3 translate);

#endif