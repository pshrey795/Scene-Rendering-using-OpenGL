#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 Normal;
out vec3 Position;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main(){
    Normal = normalMatrix * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    texCoords = aTex;
    gl_Position = projection * view * vec4(Position, 1.0);
}  