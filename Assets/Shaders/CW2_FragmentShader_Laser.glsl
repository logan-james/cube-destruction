#version 400 core

uniform sampler2D mainTex; // displacement
uniform sampler2D secondTex; // colour
uniform samplerCube mainCubeTex; // cube map

uniform vec3 cameraPos;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex
{
	vec3 worldPos;
	vec2 texCoord;
	vec3 normal;
} IN;

out vec4 fragColor;

// yToRGB method adapted from Rabbid76's answer:
// https://stackoverflow.com/questions/45750080/calculating-color-values-in-vertex-shader
// colouring based on height!
vec3 yToRGB(in float ypos)
{
	float r = 2.0 - abs(ypos * 3.0 - 4.0);
	float g = 2.0 - abs(ypos * 3.0 - 2.0);
	float b = abs(ypos * 3.0 - 3.0) - 1.0;
	
    return clamp(vec3(r, g, b), 0.0, 1.0 );
}

void main(void)
{
	vec3 I = normalize(IN.worldPos - cameraPos);
    vec3 R = reflect(I, normalize(IN.normal));
	
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	float lambert = max(0.0, dot(incident, IN.normal)); // Lambert's cosine law
	float rFactor = max(0.0, dot(halfDir, normalize(IN.normal)));
	float sFactor = pow(rFactor, 100.0);
	
	vec4 texCol = texture(secondTex, IN.texCoord);
	vec4 texCube = texture(mainCubeTex, R);
	
	vec3 ambient = texCol.rgb * lightColour * 0.1;
	vec3 diffuse = texCol.rgb * lightColour * lambert * atten;
	vec3 specular = lightColour * sFactor * atten;
	
	if (IN.normal.z > 0.0 && (IN.worldPos).z > 1.07)
	{
		vec3 colourFromY = yToRGB((IN.worldPos).z);
		fragColor = vec4(texCube.rgb + ambient + diffuse + specular, texCol.a) * vec4(colourFromY, 1.0);
	}
	else 
	{
		fragColor = vec4(texCube.rgb + ambient + diffuse + specular, texCol.a);
	}
}