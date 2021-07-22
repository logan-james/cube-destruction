#version 400 core

layout(triangles, equal_spacing, ccw) in;

uniform sampler2D heightMap;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in Vertex
{
	//vec4 colour;
	//vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	//vec3 normal; // transformed worldspace normal 
} IN[];

out Vertex
{
	//vec4 colour;
	//vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	//vec3 normal; // transformed worldspace normal 
} OUT;

void main()
{
	vec3 p0 = gl_TessCoord.x * gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_TessCoord.y * gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_TessCoord.z * gl_in[2].gl_Position.xyz;
	
	vec3 combinedPos = p0 + p1 + p2;
	vec4 worldPos = modelMatrix * vec4(combinedPos, 1);
	
	float height = texture(heightMap, OUT.texCoord).x;
	worldPos.y += height * 5;
	
	gl_Position = projMatrix * viewMatrix * worldPos;
	
	//OUT.colour = colour;
	//OUT.worldPos = worldPos.xyz;
	OUT.texCoord = IN[0].texCoord + IN[1].texCoord + IN[2].texCoord;
	//OUT.normal = normal;
}

/*
layout(triangles, equal_spacing, cw) in;

uniform sampler2D heightMap;

// out vec3 tePatchDistance;

in Vertex
{
	vec4 gl_Position;
} IN[];

out Vertex
{
	vec4 gl_Position;
} OUT;
/*
vec3 TriMixVec3(vec3 a, vec3 b, vec3 c)
{
	vec3 p0 = mix(a, c, gl_TessCoord.x);
	vec3 p1 = mix(b, a, gl_TessCoord.x);
	
	return mix(p0, p1, gl_TessCoord.y);
}

vec2 TriMixVec2(vec2 a, vec2 b, vec2 c)
{
	vec2 p0 = mix(a, c, gl_TessCoord.x);
	vec2 p1 = mix(b, a, gl_TessCoord.x);
	
	return mix(p0, p1, gl_TessCoord.y);
}
*/
/*
void main()
{
 gl_Position = projMatrix * modelMatrix * vec4(
	vec3
	(
		gl_Position.x * gl_TessCoord.x
		, gl_Position.y * gl_TessCoord.y 
		, gl_Position.z * gl_TessCoord.z
	), 1.0);
}

layout(triangles, equal_spacing, ccw) in;
in vec3 tcPosition[];

uniform float radius;
out vec3 tePosition;
out vec3 tePatchDistance;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{    
	vec3 p0 = gl_TessCoord.x * tcPosition[0];    
	vec3 p1 = gl_TessCoord.y * tcPosition[1];    
	vec3 p2 = gl_TessCoord.z * tcPosition[2];
	
	tePatchDistance = gl_TessCoord;    
	tePosition = normalize(p0 + p1 + p2) * radius;  
	
	gl_Position = projMatrix * modelMatrix * vec4(tePosition, 1);
}
*/