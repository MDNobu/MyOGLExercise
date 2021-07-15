#version 330 core
out vec4 FragColor;

uniform vec3 cameraPosition;
in vec3 normalWS;
in vec3 posWS;
uniform samplerCube cubemapSampler;

void main()
{
    //vec3 col = texture(screenTexture, TexCoords).rgb;
    //FragColor = vec4(colorTint, 1.0);
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);

    vec3 viewDir = normalize(cameraPosition - posWS);
    vec3 normal = normalize(normalWS);

    //
    //vec3 reflectionDir = reflect(-viewDir, normal);

    // refraction
    float ratio = 1.0/1.52; //1.0 是空气的IOR 1.52是玻璃的IOR
    vec3 refractionDir = refract(-viewDir, normal, ratio);
    FragColor = texture(cubemapSampler, refractionDir);
} 