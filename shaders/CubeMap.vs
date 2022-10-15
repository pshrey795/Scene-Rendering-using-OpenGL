#version 460 core
layout (location = 0) in vec3 vPos;

out vec3 texDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool isTree;

void main(){
    texDir = vPos;
    if(isTree){
        gl_Position = projection * view * model * vec4(vPos, 1.0);
    }else{
        vec4 pos = projection * view * model * vec4(vPos, 1.0);
        gl_Position = pos.xyww;
    }
}