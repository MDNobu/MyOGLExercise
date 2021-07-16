#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;



out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = aNormal;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}