#version 400 core

uniform sampler2D mainTex;
uniform samplerCube mainCubeTex; // cube map

uniform vec3 cameraPos;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform float time;
uniform int modConstant = 4000;

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
	vec4 texCol = texture(mainTex, IN.texCoord);

	if (texCol.a > 0.6)
	{
		fragColor = vec4(texCol.rgb, clamp(abs(sin(time * modConstant)), 0.0f, 1.0f));
	}
	else 
	{
		fragColor = texCol;
	}
}