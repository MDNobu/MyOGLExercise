#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    const float OFFSET_STRENGTH = 1.0;
    vec3 pos  = aPos +  aNormal * OFFSET_STRENGTH;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}