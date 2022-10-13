#include "../include/Shape.hpp"

void buildSquare(vector<Vertex> &vertices, vector<unsigned int> &indices){
    vertices.push_back(Vertex(vec3(-1.0f, -1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(vec3(1.0f, -1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(vec3(1.0f, 1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(vec3(-1.0f, 1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
}

mat4 getTransform(vec3 scaling, vec3 rotateAxis, float rotateAngle, vec3 translation){
    mat4 transform = mat4(1.0f);
    transform = translate(transform, translation);
    transform = rotate(transform, (radians(rotateAngle)), rotateAxis);
    transform = scale(transform, scaling);
    return transform;
}