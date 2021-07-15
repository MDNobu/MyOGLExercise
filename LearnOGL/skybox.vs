#version 330 core
layout (location = 0) in vec3 aPos;

//out vec2 TexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main()
{
    //TexCoords = aTexCoords;
    //gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  