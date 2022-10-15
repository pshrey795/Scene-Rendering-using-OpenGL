#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 FragPos;
out vec3 FragNorm; 

uniform mat4 model;
uniform mat4 view;
uniform mat3 normalViewMatrix;
uniform mat4 projection;

void main(){
    FragPos = vec3(model * vec4(vPos, 1.0));
    FragNorm = normalize(normalViewMatrix * vNormal);
    gl_Position = projection * view * model * vec4(vPos, 1.0);
}