#version 460 core
out vec4 fragColor;

in vec3 vNormFrag;
in vec2 vTexFrag;

uniform sampler2D tex0;

void main(){
    fragColor = texture(tex0, vTexFrag);
}