#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;



void main()
{
    
}