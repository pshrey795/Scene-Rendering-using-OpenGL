#include "../include/common.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

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
        float sun_theta = radians(-90.0f + 15.0f * hour);
        float sun_x = cos(sun_theta) * cos(radians(SUN_PHI));
        float sun_y = sin(sun_theta);
        float sun_z = cos(sun_theta) * sin(radians(SUN_PHI));
        return normalize(vec3(-sun_x, -sun_y, -sun_z));
    }else{
        return normalize(vec3(1.0f, +1.0f, -1.0f));
    }
}

vec3 convertDirToPos(vec3 dir, float dist, vec3 origin){
    return origin - dist * dir;
}

void setSTBIFlip(bool flip){
    stbi_set_flip_vertically_on_load(flip);
}

unsigned int getTextureFromFile(string fileName){
    string path = texDir + fileName;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if(data){
        GLenum format;
        if (nrComponents == 1){
            format = GL_RED;
        }else if (nrComponents == 3){
            format = GL_RGB;
        }else if (nrComponents == 4){
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }else{
        cout << "Texture failed to load at path: " << path << endl;
    }
    stbi_image_free(data);
    return textureID;
}

unsigned int getCubeMap(string fileDir){
    string pathDir = texDir + fileDir;
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    unsigned char *data;
    for(unsigned int i = 0; i < 6; i++){
        string filePath = pathDir + "/" + to_string(i+1) + ".jpg";
        data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
        GLenum format;
        if (nrComponents == 1){
            format = GL_RED;
        }else if (nrComponents == 3){
            format = GL_RGB;
        }else if (nrComponents == 4){
            format = GL_RGBA;
        }
        if(data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }else{
            cout << "Cubemap tex failed to load at path: " << filePath << endl;
        }
        stbi_image_free(data);
    }

    //Setting Parameters
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID; 
}
