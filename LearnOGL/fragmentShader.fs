#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec3 posVS;

void main()
{
    // FragColor = vec4(ourColor, 1.0);
    float brightness = ((posVS.xy + 1) * 0.5).x;
    FragColor = vec4(brightness, brightness, brightness , 1.0);
}