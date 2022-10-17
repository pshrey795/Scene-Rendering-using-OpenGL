#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 texCoords;

uniform bool isDay;
#define NUM_LAMPS 24 
uniform vec3 lampPos[NUM_LAMPS];

uniform vec3 cameraPos;
uniform sampler2D pebbleTexture;
uniform sampler2D lakeTexture;
uniform samplerCube skybox;

uniform float blendFactor;

float calcPointLight(vec3 lightPos, vec3 normal){
    float finalColor = 0.0;
    finalColor += 0.1;
    vec3 lightDir = normalize(lightPos - Position);
    vec3 norm = normalize(normal);
    float cosine = max(dot(norm, lightDir), 0.0);
    finalColor += cosine;

    //Calculating Attenuation 
    float distance = length(lightPos - Position);
    float attenuation = 1.0 / (1.0 + 0.009 * distance);// + 0.032 * distance * distance);
    finalColor *= attenuation;
    
    return finalColor;
}

void main(){  
    vec3 pebble = texture(pebbleTexture, texCoords).rgb;
    vec3 water = texture(lakeTexture, texCoords).rgb;
    vec3 color = water * (1 - blendFactor) + pebble * blendFactor;
    if(isDay){         
        FragColor = vec4(color, 1.0);
    }else{
        float finalColor = 0.0;
        for(int i = 0; i < NUM_LAMPS; i++){
            finalColor += calcPointLight(lampPos[i], Normal);
        }
        FragColor = vec4(color * finalColor, 1.0);
    }
}