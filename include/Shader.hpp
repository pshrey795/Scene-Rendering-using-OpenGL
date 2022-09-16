#ifndef SHADER_HPP
#define SHADER_HPP

#include "common.hpp"

using namespace std;

class Shader {

    public:
        Shader(string vertexPath, string fragmentPath);
        void use();

    private:
        unsigned int shaderID;

};

#endif