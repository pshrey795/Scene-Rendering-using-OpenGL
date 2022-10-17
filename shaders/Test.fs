#version 460 core
out vec4 FragColor;
  
in vec3 FragPos;
in vec3 FragNorm; 
in vec2 FragTexCoords;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, FragTexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
} 