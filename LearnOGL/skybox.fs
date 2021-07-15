#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube skyboxSampler;

void main()
{
    //vec3 col = texture(screenTexture, TexCoords).rgb;
    //FragColor = vec4(colorTint, 1.0);
    FragColor = texture(skyboxSampler, TexCoords);

    // float test = ( TexCoords.x + 1) * 0.5;
    // vec3 tmp  = vec3(test);
    // FragColor = vec4(tmp, 1.0);
} 
