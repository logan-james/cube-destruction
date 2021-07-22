#version 400 core


uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);
uniform vec3 cameraPos;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 texCoord;
layout(location = 3) in vec3 normal;

out Vertex
{
	vec3 normal;
	vec3 texCoord;
} OUT;

void main()
{
	mat4 newView = mat4(mat3(viewMatrix));
	
	vec3 multiply = vec3(0, 0, 0);
	multiply.x = 1.0f / projMatrix[0][0];
	multiply.y = 1.0f / projMatrix[1][1];

	vec3 tempPos = (position * multiply) - vec3(0, 0, 1);
	OUT.normal = transpose(mat3(viewMatrix)) * normalize(tempPos);
	
	// output
	OUT.texCoord = position.xyz;	
	gl_Position = projMatrix * newView * vec4(position, 1.0);
}