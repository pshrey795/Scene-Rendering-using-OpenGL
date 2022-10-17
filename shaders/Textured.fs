#version 460 core
out vec4 fragColor;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

uniform bool isDay;
uniform sampler2D tex0;

void main(){
    fragColor = texture(tex0, FragTex);
}