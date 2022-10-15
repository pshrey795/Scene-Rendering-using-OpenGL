#version 460 core
out vec4 fragColor;

uniform bool isDay;

void main(){
    if(!isDay){
        fragColor = vec4(1.0f);
    }else{
        fragColor = vec4(vec3(0.0f), 1.0);
    }
}