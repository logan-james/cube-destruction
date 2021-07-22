#version 400 core

uniform sampler2D mainTex;
uniform sampler2D secondTex;
uniform samplerCube mainCubeTex; // cube map

uniform vec3 cameraPos;
uniform vec3 lightColour;
uniform vec3 lightPos;

uniform float lightRadius;
uniform float time;
uniform int blendSpeed = 150;

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
	float alphaBlend = clamp(time * blendSpeed, 0.0, 1.0);
	
	float lambert = max(0.0, dot(incident, IN.normal)); // Lambert's cosine law
	float rFactor = max(0.0, dot(halfDir, normalize(IN.normal)));
	float sFactor = pow(rFactor, 50.0);
	
	vec4 texMainCol = texture(mainTex, IN.texCoord);
	vec4 texSecCol = texture(secondTex, IN.texCoord);
	vec4 texFinal = mix(texMainCol, texSecCol, alphaBlend);
	
	vec3 ambient = texFinal.rgb * lightColour * 0.1;
	vec3 diffuse = texFinal.rgb * lightColour * lambert * atten;
	vec3 specular = lightColour * sFactor * atten;
	
	fragColor = vec4(ambient + diffuse + specular, texFinal.a);
}