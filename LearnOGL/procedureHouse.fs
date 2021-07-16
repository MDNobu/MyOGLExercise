
#version 330 core

in vec3 fColor;

void main()
{    
    // FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    gl_FragColor = vec4(fColor, 1.0);
}