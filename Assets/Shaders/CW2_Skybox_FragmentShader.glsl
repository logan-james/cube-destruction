#version 400 core

uniform samplerCube mainCubeTex;
uniform vec3 cameraPos;

in Vertex
{
	vec3 normal;
	vec3 texCoord;
} IN;

out vec4 fragColor;

void main(void)
{
	// fragColor = texture(mainCubeTex, normalize(IN.normal));
	fragColor = texture(mainCubeTex, IN.texCoord);
}