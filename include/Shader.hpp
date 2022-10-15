#ifndef SHADER_HPP
#define SHADER_HPP

#include "common.hpp"

using namespace std;

class Shader {

    public:
        Shader(string vertexPath, string fragmentPath);
        void use();
        void setMat3(const string &name, mat3 value);
        void setMat4(const string &name, mat4 value);
        void setFloat(const string &name, float value);
        void setInt(const string &name, int value);
        void setVec3(const string &name, vec3 value);
        void setBool(const string &name, bool value);

    private:
        unsigned int shaderID;

};

#endif