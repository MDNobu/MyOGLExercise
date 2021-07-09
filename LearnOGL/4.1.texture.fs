#version 330 core
out vec4 FragColor;

// in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 colorTint;

void main()
{
	// transform uv for texture2
	// TexCoord.y = 1 - TexCoord.y;
	vec2 uv = vec2(TexCoord.x,  TexCoord.y);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, uv), 0.5) ;
	// FragColor = texture(texture1, TexCoord);
	//FragColor *= colorTint;
}