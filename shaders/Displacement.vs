#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aTang;
layout (location = 4) in vec3 aBitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalViewMatrix;

out vec3 FragPos;
out vec2 FragTex;
out vec3 FragNorm;
out mat3 TBN;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragTex = aTex;
    FragNorm = normalize(normalViewMatrix * aNorm);
    
    vec3 T = normalize(normalViewMatrix * aTang);
    vec3 N = normalize(normalViewMatrix * aNorm);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
} 