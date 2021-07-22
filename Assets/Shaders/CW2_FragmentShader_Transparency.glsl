#version 400 core

uniform sampler2D mainTex; // displacement
uniform sampler2D secondTex; // colour
uniform samplerCube mainCubeTex; // cube map

uniform vec3 cameraPos;
uniform vec3 lightColour;
uniform vec3 lightPos;

uniform float lightRadius;
uniform float time;
uniform int blendSpeed = 200;

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
	vec3 I = normalize(IN.worldPos - cameraPos);
    vec3 R = reflect(I, normalize(IN.normal));

	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	float alphaBlend = 1.0 - clamp(time * blendSpeed, 0.0, 1.0);
	
	float lambert = max(0.0, dot(incident, IN.normal)); // Lambert's cosine law
	float rFactor = max(0.0, dot(halfDir, normalize(IN.normal)));
	float sFactor = pow(rFactor, 50.0);
	
	vec4 texCol = texture(secondTex, IN.texCoord);
	vec4 texCube = texture(mainCubeTex, R);
		
	vec3 ambient = texCol.rgb * lightColour * 0.1;
	vec3 diffuse = texCol.rgb * lightColour * lambert * atten;
	vec3 specular = lightColour * sFactor * atten;
	
	vec3 finalColour = texCube.rgb + ambient + diffuse + specular;

	fragColor = vec4(finalColour, alphaBlend);
}