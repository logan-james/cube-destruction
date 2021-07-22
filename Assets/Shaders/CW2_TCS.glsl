#version 400 core
layout(vertices = 3) out; // num vertices in output patch

uniform float tessLevelInner = 20;
uniform float tessLevelOuter = 20;

// from vertex shader
in Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} IN[]; // equal to the size of the draw call vertex count

 // to evaluation shader
out Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} OUT[]; // equal to the size of the layout vertex count 

void main()
{
	gl_TessLevelInner[0] = tessLevelInner;

	gl_TessLevelOuter[0] = tessLevelInner;
	gl_TessLevelOuter[1] = tessLevelInner;
	gl_TessLevelOuter[2] = tessLevelInner;
	
	barrier();

	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].worldPos = IN[gl_InvocationID].worldPos;
	OUT[gl_InvocationID].normal = IN[gl_InvocationID].normal;
	OUT[gl_InvocationID].colour = IN[gl_InvocationID].colour;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
