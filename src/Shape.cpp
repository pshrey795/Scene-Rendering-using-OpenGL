#include "../include/Shape.hpp"

void buildCylinder(vector<Vertex> &vertices, vector<unsigned int> &indices){
    int n=360;

    for (int i=0;i<n;i++){
        double theta= (2*(M_PI)*i)/n;
        double theta_nxt= (2*(M_PI)*(i+1))/n;
        vertices.push_back(Vertex(vec3(1.0f, 0.0f, 0.0),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta), 0.5*cos(theta)),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  vec3(1.0f, 0.0f, 0.0f)));

        vertices.push_back(Vertex(vec3(-1.0f, 0.0f, 0.0),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta), 0.5*cos(theta)),  vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  vec3(1.0f, 0.0f, 0.0f)));

        vec3 normal= vec3(0, 0.5*(cos(theta_nxt)-cos(theta)), 0.5*(sin(theta_nxt)-sin(theta)));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta), 0.5*cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta), 0.5*cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  normal));
        
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta), 0.5*cos(theta)),  normal));
        vertices.push_back(Vertex(vec3(1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  normal));
        vertices.push_back(Vertex(vec3(-1.0f, -0.5*sin(theta_nxt), 0.5*cos(theta_nxt)),  normal));
    }

    for (int i=0;i<12*n;i++){
        indices.push_back(i);
    }
}

void buildSquare(vector<Vertex> &vertices, vector<unsigned int> &indices){
    vertices.push_back(Vertex(vec3(-1.0f, -1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)));
    vertices.push_back(Vertex(vec3(1.0f, -1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)));
    vertices.push_back(Vertex(vec3(1.0f, 1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)));
    vertices.push_back(Vertex(vec3(-1.0f, 1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
}