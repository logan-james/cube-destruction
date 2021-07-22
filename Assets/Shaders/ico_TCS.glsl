#version 400 core
layout(vertices = 3) out; // num vertices in output patch

uniform float tessLevelInner = 3;
uniform float tessLevelOuter = 3;

in Vertex
{
	vec4 colour; // from vertex shader
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} IN[]; // equal to the size of the draw call vertex count

out Vertex
{
	vec4 colour; // to evaluation shader
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} OUT[]; // equal to the size of the layout vertex count 

patch out Vector4 subColour;

void main()
{
	gl_TessLevelInner[0] = tessLevelInner;
	// gl_TessLevelInner[1] = tessLevelInner;     //quads only !
	
	gl_TessLevelOuter[0] = tessLevelInner;
	gl_TessLevelOuter[1] = tessLevelInner;
	gl_TessLevelOuter[2] = tessLevelInner;
	// gl_TessLevelOuter[3] = tessLevelInner;     // quads only !
	
	barrier();

	OUT[gl_InvocationID].texCoord = IN[gl_InvocationID].texCoord;
	OUT[gl_InvocationID].worldPos = IN[gl_InvocationID].worldPos;
	OUT[gl_InvocationID].normal = IN[gl_InvocationID].normal;
	OUT[gl_InvocationID].colour = IN[gl_InvocationID].colour;
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}

/*
#version 400 core

layout(vertices = 3) out; // num vertices in output patch

in Vertex
{
	vec4 gl_Position; // from vertex shader
} IN[]; // equal to the size of the draw call vertex count

out Vertex
{
	vec4 gl_Position; // to evaluation shader
} OUT[]; // equal to the size of the layout vertex count 

void main()
{
	gl_TessLevelInner[0] = 32;
	gl_TessLevelInner[1] = 32;
	
	gl_TessLevelOuter[0] = 32;
	gl_TessLevelOuter[1] = 32;
	gl_TessLevelOuter[2] = 32;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
*/

/*
layout(vertices = 3) out;
in vec3 vPosition[];
out vec3 tcPosition[];

#define ID gl_InvocationID

void main()
{    
	tcPosition[ID] = vPosition[ID];    
	if(ID == 0)
	{        
		gl_TessLevelInner[0] = innerTess;
		
		gl_TessLevelOuter[0] = outerTess;
		gl_TessLevelOuter[1] = outerTess;
		gl_TessLevelOuter[2] = outerTess;    
	}
}
*/