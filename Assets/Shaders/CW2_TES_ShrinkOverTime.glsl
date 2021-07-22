#version 410 core

layout(triangles, equal_spacing, ccw) in;

uniform sampler2D mainTex; // displacement

uniform float time = 0.0f;
uniform float heightFactor = 0.5;

uniform int scaleConstant = 400;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

// from the TCS
in Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} IN[];

// outputs one vertex
out Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} OUT;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return
		vec2(gl_TessCoord.x) * v0 
		+ vec2(gl_TessCoord.y) * v1 
		+ vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return
		vec3(gl_TessCoord.x) * v0 
		+ vec3(gl_TessCoord.y) * v1 
		+ vec3(gl_TessCoord.z) * v2;
}

void main()
{
	mat4 scaleMatrix = mat4(clamp(pow(2, (time * -1 * scaleConstant)), 0.0, 1.0));
	scaleMatrix[3][3] = 1.0;
	
    OUT.texCoord = interpolate2D(IN[0].texCoord, IN[1].texCoord, IN[2].texCoord);
    OUT.normal = normalize(interpolate3D(IN[0].normal, IN[1].normal, IN[2].normal));
    OUT.worldPos = interpolate3D(IN[0].worldPos, IN[1].worldPos, IN[2].worldPos);
	
	OUT.colour = mix(IN[0].colour, IN[1].colour, IN[2].colour);
	
	gl_Position = projMatrix * viewMatrix * modelMatrix * scaleMatrix * vec4(OUT.worldPos, 1.0);
}
