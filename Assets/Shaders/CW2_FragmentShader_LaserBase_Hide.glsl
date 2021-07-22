#version 400 core

// just make it disappear!!

uniform sampler2D mainTex;
uniform samplerCube mainCubeTex; // cube map

uniform vec3 cameraPos;

uniform float time;


in Vertex
{
	vec4 colour;
	vec3 worldPos;
	vec2 texCoord;
	vec3 normal;
} IN;

out vec4 fragColor;

void main(void)
{
	fragColor = vec4(IN.colour.rgb, 0); // * texCube;
}