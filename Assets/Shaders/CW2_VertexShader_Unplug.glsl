#version 400 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

uniform float time = 0.0f;
uniform int spinConstant = 500;
uniform int speed = 2500;

uniform int modConstant = 100;
uniform int scaleConstant = 30;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

out Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} OUT;

void main(void)
{	
	mat4 rotMatrix = mat4(1.0 / (time * scaleConstant));
	rotMatrix[3][3] = 1.0;
	
	vec4 worldPos = vec4(position, 1.0);
	gl_Position = (projMatrix * viewMatrix) * worldPos;
	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	
	OUT.texCoord = texCoord;
	OUT.worldPos = worldPos.xyz;
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.colour = colour;
}