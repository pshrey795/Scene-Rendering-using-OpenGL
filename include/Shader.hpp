#ifndef SHADER_HPP
#define SHADER_HPP

#include "common.hpp"

using namespace std;

class Shader {

    public:
        Shader(string vertexPath, string fragmentPath);
        void use();
        void setMat4(const string &name, mat4 value);

    private:
        unsigned int shaderID;

};

#endif