#ifndef COMMON_HPP
#define COMMON_HPP

#include<bits/stdc++.h>
#include "../external/glad.h"
#include "GLFW/glfw3.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#define DELTA_TIME 0.01f

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

#endif