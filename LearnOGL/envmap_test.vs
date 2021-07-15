#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normalLS;

//out vec2 TexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normalWS;
out vec3 posWS;

void main()
{
    //TexCoords = aTexCoords;
    //gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    //TexCoords = aPos;
    posWS = (model * vec4(aPos, 1.0)).xyz;

    // tranform normal to WS
    // 这里正常来说是在CPU进行的，这里简化
    vec4 tmp = transpose(inverse(model)) * vec4(normalLS, 0.0);
    normalWS = tmp.xyz;
    gl_Position =  projection * view * model * vec4(aPos, 1.0);
}  