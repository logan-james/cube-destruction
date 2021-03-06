#version 400 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

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
	vec4 worldPos = modelMatrix * vec4(position, 1.0);
	gl_Position = vec4(0, 0, 0, 1); //(projMatrix * viewMatrix) * worldPos;
	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	
	OUT.texCoord = texCoord;
	OUT.worldPos = worldPos.xyz;
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.colour = colour;
}