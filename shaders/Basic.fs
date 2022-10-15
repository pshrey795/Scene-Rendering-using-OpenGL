#version 460 core
out vec4 fragColor;

in vec3 FragPos;
in vec3 FragNorm;

uniform bool isDay;
uniform vec3 sunDir;  //Sun as a directional light
#define MAX_LAMPS 30
uniform int numLamps;
uniform vec3 lampPos[MAX_LAMPS];

uniform vec3 viewPos; //Camera position 
uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;
uniform float specularity;

vec3 calcDirLight(vec3 normal, vec3 viewDir){
    vec3 finalColor = ambient;
    float cosine = max(dot(normal, -sunDir), 0.0);
    finalColor += (diffuse * cosine);
    vec3 reflectDir = normalize(reflect(sunDir, normal));
    float cosineSpec = pow(max(dot(viewDir, reflectDir), 0.0), specularity);
    finalColor += (specular * cosineSpec);
    return finalColor;
}

vec3 calcPointLight(vec3 lightPos, vec3 normal, vec3 viewDir){
    vec3 finalColor = ambient;
    vec3 lightDir = normalize(lightPos - FragPos);
    float cosine = max(dot(normal, lightDir), 0.0);
    finalColor += diffuse * cosine;
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float cosineSpec = pow(max(dot(viewDir, reflectDir), 0.0), specularity);
    finalColor += specular * cosineSpec;

    //Calculating Attenuation 
    float distance = length(lightDir);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    finalColor *= attenuation;
    
    return finalColor;
}

void main(){
    vec3 viewDir = normalize(viewPos-FragPos);
    if(!isDay){
        fragColor = vec4(calcDirLight(FragNorm, viewDir), 1.0);
    }else{
        vec3 finalColor = vec3(0.0, 0.0, 0.0);
        for(int i = 0; i < numLamps; i++){
            finalColor += calcPointLight(lampPos[i], FragNorm, viewDir);
        }
        fragColor = vec4(finalColor, 1.0);
    }
}