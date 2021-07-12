#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormalMS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvp;

out vec3 aNormalWS;
out vec3 aPosWS;
void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = mvp * vec4(aPos, 1.0);

	// 这里计算normal的方法对应non-uniform scale不正确，
	// 应该用逆转置，暂时这样是为了简化运算
	aNormalWS = (model * vec4(aNormalMS, 0.0)).xyz;

	aPosWS = (model * vec4(aPos, 1.0)).xyz;
}