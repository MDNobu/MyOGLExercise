#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    //vec3 color;
    vec3 normal;
    vec2 uv;
} vs_out;

void main()
{
    //vs_out.color = aNormal;
    vs_out.normal = aNormal;
    vs_out.uv = aUV;
    //vs_out.
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}