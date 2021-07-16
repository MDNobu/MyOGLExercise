#version 330 core

uniform sampler2D texture_diffuse1;

in VS_OUT {
    //vec3 color;
    vec3 normal;
    vec2 uv;
} vs_out;

void main()
{
    vec4 res = texture(texture_diffuse1, vs_out.uv);
    gl_FragColor = res;
}