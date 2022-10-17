#ifndef COMMON_HPP
#define COMMON_HPP

#include<bits/stdc++.h>
#include "../external/glad.h"
#include "GLFW/glfw3.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define DELTA_TIME 0.01f
#define ENABLE_SUN 1
#define SUN_COLOR vec3(1.0f, 1.0f, 1.0f)
#define SUN_PHI 0.0f
#define LAMP_COLOR vec3(1.0f, 1.0f, 1.0f)
#define LAMP_MAX 30

using namespace std;
using namespace glm;

extern string texDir;
extern string objDir;
extern string shaderDir;

//Overload << operator for vec2, vec3
std::ostream& operator<<(std::ostream& os, const vec2& v);
std::ostream& operator<<(std::ostream& os, const vec3& v);

//Overload * operator for vec2, vec3
vec2 operator*(const vec2& v, const double& f);
vec2 operator*(const double& f, const vec2& v);
vec3 operator*(const vec3& v, const double& f);
vec3 operator*(const double& f, const vec3& v);

//Getting the direction of sunlight based on the time of day
vec3 getSunDir();
vec3 convertDirToPos(vec3 dir, float dist = 100.0f, vec3 origin = vec3(0.0f));

//STBI related functions
void setSTBIFlip(bool flip);

//Extracting new textures
unsigned int getTextureFromFile(string fileName);
unsigned int getCubeMap(string fileDir);

#endif