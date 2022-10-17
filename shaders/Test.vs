#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 FragPos;
out vec3 FragNorm; 
out vec2 FragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat3 normalViewMatrix;
uniform mat4 projection;

void main(){
    FragPos = vec3(model * vec4(vPos, 1.0));
    FragNorm = normalize(normalViewMatrix * vNormal);
    FragTexCoords = vTexCoords;
    gl_Position = projection * view * model * vec4(vPos, 1.0);
}