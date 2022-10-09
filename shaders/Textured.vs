#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormFrag;
out vec2 vTexFrag;

void main(){
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    vNormFrag = vNorm;
    vTexFrag = vTex;
}