#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;

uniform float ambientIntensity;
uniform vec3 uLightPosWS;
uniform float shineness;
uniform vec3 lightColor;
in vec3 aNormalWS;
in vec3 aPosWS;
void main()
{
    
    
    vec3 normalWS = normalize(aNormalWS);
    vec3 lightDirWS = normalize(uLightPosWS - aPosWS);

    float ambientFactor = ambientIntensity * 0.05;
    vec3 reflectWS = reflect(-lightDirWS, normalWS);
    float diffuseFactor = max(dot(normalWS, lightDirWS), 0.0);
    const float specStrength = 2.0f;
    // float specularFactor = pow(max(dot(normalWS,reflectWS), 0.0), shineness);
    float specularFactor = specStrength * pow(max(dot(normalWS,reflectWS), 0.0), shineness);

    //specularFactor *= diffuseFactor;


    float lightness = ambientFactor  + diffuseFactor + specularFactor;
    lightness = max(dot(normalWS,reflectWS), 0.0);
    //lightness = clamp(lightness, 0.0, 1.0);
    vec3 resColor = ambientFactor * objectColor + 
        (diffuseFactor + specularFactor) * (lightColor * objectColor);
   // FragColor = lightness * vec4(lightColor * objectColor, 1.0);
    FragColor = vec4(resColor, 1.0);
    //FragColor = vec4(lightness, lightness, lightness, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0);
}