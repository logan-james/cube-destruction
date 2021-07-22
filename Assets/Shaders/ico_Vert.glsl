#version 400 core

/*
layout(location = 0) in vec3 position;

out Vertex
{
	vec4 gl_Position;
} OUT;

void main(void)
{
	gl_Position = vec4(position, 1.0);
}
*/

in vec3 in_Position;
in vec3 in_Normal;
in vec3 in_TexCoord;
out vec3 vPosition;

void main()
{
	vPosition = in_Position;
}