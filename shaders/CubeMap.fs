#version 460 core
out vec4 fragColor;

in vec3 texDir;
uniform samplerCube cubeMap;
uniform bool isDay;
uniform bool isTree; 
uniform vec3 color; 

void main(){
    if(isDay){
        if(isTree){
            fragColor = vec4(color, 1.0);
        }else{
            fragColor = texture(cubeMap, texDir);
        }
    }else{
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}