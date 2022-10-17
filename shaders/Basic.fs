#version 460 core
out vec4 fragColor;

in vec3 FragPos;
in vec3 FragNorm;

uniform bool isDay;
uniform vec3 sunDir;  //Sun as a directional light

#define NUM_LAMPS 24 
uniform vec3 lampPos[NUM_LAMPS];

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
    vec3 finalColor = vec3(0.0f);
    finalColor += ambient;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(normal);
    float cosine = max(dot(norm, lightDir), 0.0);
    finalColor += diffuse * cosine;
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    float cosineSpec = pow(max(dot(viewDir, reflectDir), 0.0), specularity);
    finalColor += specular * cosineSpec;

    //Calculating Attenuation 
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.002 * distance * distance);
    finalColor *= attenuation;
    
    return finalColor;
}

void main(){
    vec3 viewDir = normalize(viewPos-FragPos);
    if(isDay){
        fragColor = vec4(calcDirLight(FragNorm, viewDir), 1.0);
    }else{
        vec3 finalColor = vec3(0.0, 0.0, 0.0);
        for(int i=0;i<24;i++){
            finalColor += calcPointLight(lampPos[i], FragNorm, viewDir);
        }
        fragColor = vec4(finalColor, 1.0);
    }
}