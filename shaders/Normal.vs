#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTex;
layout (location = 3) in vec3 vTang;
layout (location = 4) in vec3 vBitang;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;
uniform mat3 normalViewMatrix;

out vec3 FragPos;
out vec2 FragTex;
out vec3 FragNorm;
out mat3 TBN;

void main(){
    FragPos = vec3(model * vec4(vPos, 1.0));
    FragTex = vTex;
    FragNorm = normalize(normalViewMatrix * vNorm);

    vec3 T = normalize(normalViewMatrix * vTang);
    vec3 N = normalize(normalViewMatrix * vNorm);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    gl_Position = projection * view * model * vec4(vPos, 1.0);
}
