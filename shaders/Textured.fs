#version 460 core
out vec4 fragColor;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

uniform sampler2D tex0;
uniform bool isDay;
uniform vec3 sunDir; 
#define MAX_LAMPS 30
uniform int numLamps;
uniform vec3 lampPos[MAX_LAMPS];

float calcDirLight(vec3 normal){
    float cosine = dot(normal, -sunDir);
    return cosine;
}

float calcPointLight(vec3 lightPos){
    vec3 lightDir = normalize(lightPos - FragPos);
    float cosine = max(dot(FragNorm, lightDir), 0.0);
    float distance = length(lightDir);
    float attenuation = 1.0 / (1.0 + 0.09 * distance);
    return cosine * attenuation;
}

void main(){
    vec4 texColor = texture(tex0, FragTex);
    float finalColor = 0.0;
    if(!isDay){
        finalColor = calcDirLight(FragNorm);
    }else{
        for(int i=0;i<numLamps;i++){
            finalColor = finalColor + calcPointLight(lampPos[i]);
        }
    }
    fragColor = vec4(finalColor * texColor.rgb, texColor.a);
}