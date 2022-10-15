#version 460 core
out vec4 fragColor;

in vec3 texDir;
uniform samplerCube cubeMap;

uniform bool isTree; 
uniform vec3 color; 

void main(){
    if(isTree){
        fragColor = vec4(color, 1.0);
    }else{
        fragColor = texture(cubeMap, texDir);
    }
}