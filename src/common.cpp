#include "../include/common.hpp"

string texDir = "assets/textures/";
string objDir = "assets/objects/";
string shaderDir = "shaders/";

int numTex = 0;

//Overload << operator for vec2, vec3
std::ostream& operator<<(std::ostream& os, const vec2& v){
    os << "(" << v[0] << ", " << v[1] << ")";
    return os;
}
std::ostream& operator<<(std::ostream& os, const vec3& v){
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
    return os;
}

//Overload * operator for vec2, vec3
vec2 operator*(const vec2& v, const double& f){
    return vec2(v[0]*f, v[1]*f);
}
vec2 operator*(const double& f, const vec2& v){
    return vec2(v[0]*f, v[1]*f);
}
vec3 operator*(const vec3& v, const double& f){
    return vec3(v[0]*f, v[1]*f, v[2]*f);
}
vec3 operator*(const double& f, const vec3& v){
    return vec3(v[0]*f, v[1]*f, v[2]*f);
}
