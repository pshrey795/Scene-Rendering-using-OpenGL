#version 460 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 FragTex;
in vec3 FragNorm;
in mat3 TBN;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform bool isDay;
#define NUM_LAMPS 24 
uniform vec3 lampPos[NUM_LAMPS];
uniform vec3 lightDir;

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
    vec3 normal = texture(normalTexture, FragTex).rgb;
    normal = normal * 2.0 - vec3(1.0, 1.0, 1.0);
    normal = normalize(TBN * normal);

    vec3 color = texture(diffuseTexture, FragTex).rgb;

    if(isDay){
        float ambient = 0.1;
        float cosine = max(dot(normal, -lightDir), 0.0);

        vec3 finalColor = (ambient + cosine) * color;
        FragColor = vec4(finalColor, 1.0);
    }else{
        float finalColor = 0.0;
        for(int i = 0; i < NUM_LAMPS; i++){
            finalColor += calcPointLight(lampPos[i], normal);
        }
        FragColor = vec4(finalColor * color, 1.0);
    }
}