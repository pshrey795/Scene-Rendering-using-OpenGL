#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 FragTex;
in vec3 FragNorm;
in mat3 TBN;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
  
uniform float height_scale;
uniform vec3 lightDir;
uniform vec3 viewPos;

uniform bool isDay;
#define NUM_LAMPS 24 
uniform vec3 lampPos[NUM_LAMPS];

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir){ 
    float height =  texture(depthMap, texCoords).r;    
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    return texCoords - p;    
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
    vec3 viewDir   = normalize(viewPos - FragPos);
    // viewDir = transpose(TBN) * viewDir;
    vec2 texCoords = ParallaxMapping(FragTex,viewDir);
    vec3 color = texture(depthMap, texCoords).rgb;
    vec3 normal  = texture(normalMap, texCoords).rgb;
    normal = normal * 2.0 - vec3(1.0, 1.0, 1.0);
    normal = normalize(TBN * normal);
    if(isDay){      
        FragColor = vec4(color, 1.0);
    }else{
        float finalColor = 0.0;
        for(int i = 0; i < NUM_LAMPS; i++){
            finalColor += calcPointLight(lampPos[i], FragNorm);
        }
        FragColor = vec4(color * finalColor, 1.0);
    }
}