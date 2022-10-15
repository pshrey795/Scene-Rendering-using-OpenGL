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

//Getting the direction of sunlight based on the time of day
vec3 getSunDir(){
    if(ENABLE_SUN){
        //Getting current time of day using chrono
        auto currentTime = chrono::system_clock::now();
        //Extract time from currentTime
        time_t time = chrono::system_clock::to_time_t(currentTime);
        //Extract the current hour from time
        int hour = localtime(&time)->tm_hour;
        float sun_theta = -90.0f + 15.0f * hour;
        float sun_x = cos(sun_theta) * cos(SUN_PHI);
        float sun_y = sin(sun_theta);
        float sun_z = cos(sun_theta) * sin(SUN_PHI);
        return normalize(vec3(-sun_x, -sun_y, -sun_z));
    }else{
        return normalize(vec3(1.0f, +1.0f, -1.0f));
    }
}
