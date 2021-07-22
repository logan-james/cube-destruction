#version 400 core

uniform float time;
uniform int timeConst = 400;
uniform float particleSize = 1.0f;
uniform float explodeFactor = 0.2;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform vec3 cameraPos;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} IN[];

out Vertex
{
	vec4 colour;
	vec3 worldPos; // place in the worldPos
	vec2 texCoord;
	vec3 normal; // transformed worldspace normal 
} OUT;

vec4 explodePoints(vec4 pos, vec3 normal)
{
    vec3 direction = normal * time * timeConst; 
    return pos + vec4(direction, 0.0);
}

void  main()
{
	// Calculate two vectors in the plane of the input triangle
	vec3 ab = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 ac = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 currentNormal = normalize(cross(ab, ac));

	for(int i = 0; i < gl_in.length(); ++i)
	{	
		vec3 currentPos = gl_in[i].gl_Position.xyz;

		vec3 worldPosNormal = mix(IN[0].worldPos, IN[1].worldPos, IN[2].worldPos);
		vec4 newPos = modelMatrix * explodePoints(
										vec4(
											mix(currentPos, normalize(currentPos), vec3(1.0, 1.0, 1.0))
										, 1.0), normalize(worldPosNormal));
		
		gl_Position = projMatrix * viewMatrix * newPos;

		OUT.texCoord = IN[i].texCoord;
		OUT.colour = IN[i].colour;
		OUT.normal = currentNormal;
		OUT.worldPos = newPos.xyz;	

		EmitVertex();
	}
	
	EndPrimitive();
}