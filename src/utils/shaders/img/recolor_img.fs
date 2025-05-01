#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 targetColor;



void main()
{
	vec4 texColor = texture(texture1, TexCoords);
	FragColor = vec4(targetColor * texColor.rgb, texColor.a);
}