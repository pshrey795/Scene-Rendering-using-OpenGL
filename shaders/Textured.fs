#version 460 core
out vec4 fragColor;

in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec3 FragNorm;
in vec2 FragTex;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool isDay;
#define NUM_LAMPS 24 
uniform vec3 lampPos[NUM_LAMPS];

float calculateShadow(vec4 lightSpaceFragPos){
    //Normalized Device Coordinates to Clip Space Coordinates
    vec3 projCoords = lightSpaceFragPos.xyz / lightSpaceFragPos.w;
    //Changing Range to [0,1]
    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = (currentDepth > closestDepth) ? 0.0 : 1.0;
    return shadow;
}

float calcPointLight(vec3 lightPos, vec3 normal){
    float finalColor = 0.0;
    finalColor += 0.1;
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(normal);
    float cosine = max(dot(norm, lightDir), 0.0);
    finalColor += cosine;

    //Calculating Attenuation 
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance);// + 0.032 * distance * distance);
    finalColor *= attenuation;
    
    return finalColor;
}

void main(){
    if(isDay){
        vec4 color = texture(diffuseTexture, FragTex);
        vec3 normal = FragNorm;

        //Ambient
        vec3 ambient = vec3(0.1, 0.1, 0.1);

        //Diffuse
        vec3 lightDir = normalize(lightPos - FragPos);
        float cosine = max(dot(normal, lightDir), 0.0);

        //Specular 
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float specularCosine = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        //Shadow
        float shadow = calculateShadow(FragPosLightSpace);

        vec3 finalColor = (ambient + shadow * (cosine + specularCosine)) * color.rgb;
        fragColor = vec4(finalColor, color.a);
    }else{
        vec4 color = texture(diffuseTexture, FragTex);
        float finalColor = 0.0;
        for(int i = 0; i < NUM_LAMPS; i++){
            finalColor += calcPointLight(lampPos[i], FragNorm);
        }
        fragColor = vec4(finalColor * color.rgb, color.a);
    }
}