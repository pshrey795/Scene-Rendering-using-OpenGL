#include "../include/Shader.hpp"

Shader::Shader(string vertexPath, string fragmentPath){
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
    try {
        vShaderFile.open(shaderDir + vertexPath);
        fShaderFile.open(shaderDir + fragmentPath);
        stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();	
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }catch(ifstream::failure e){
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << vertexPath << ": ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragment,512,NULL,infoLog);
        cout << fragmentPath << ": ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //Shader Program
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertex);
    glAttachShader(this->shaderID, fragment);
    glLinkProgram(this->shaderID);

    glGetProgramiv(this->shaderID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->shaderID,512,NULL,infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    //Delete Shaders after registering them to the program 
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(){
    glUseProgram(shaderID);
}

void Shader::setMat3(const string &name, mat3 value){
    glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void Shader::setMat4(const string &name, mat4 value){
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, value_ptr(value));
}

void Shader::setFloat(const string &name, float value){
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setInt(const string &name, int value){
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setVec3(const string &name, vec3 value){
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, value_ptr(value));
}

void Shader::setBool(const string &name, bool value){
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}