// inspired by: https://ominousgames.com/2017/12/29/the-great-disappearing-act-making-of-a-possession-effect/ 

#version 400 core

uniform sampler2D mainTex; // displacement
uniform sampler2D secondTex; // colour
uniform sampler2D thirdTex; // grid
uniform samplerCube mainCubeTex; // cube map

uniform float time;
uniform int blendSpeed = 200;
uniform float dissolveBand = 0.25;
uniform vec4 glowSetCol = vec4(0.2, 1, 0, 1); // lime green
// begin and end of glow effect
// this is in the mesh's model view
uniform vec4 meshEnd = vec4(1, 1, 0, 1);
uniform vec4 meshStart = vec4(-1, -1, 0, 1);

uniform vec3 cameraPos;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

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
	// reflection from skycube
	vec3 I = normalize(IN.worldPos - cameraPos);
    vec3 R = reflect(I, normalize(IN.normal));
	
	// lighting
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	
	float lambert = max(0.0, dot(incident, IN.normal)); // Lambert's cosine law
	float rFactor = max(0.0, dot(halfDir, normalize(IN.normal)));
	float sFactor = pow(rFactor, 100.0);

	// define some scales for the glow -- this is over time
	float dissolveScale = clamp(time * blendSpeed, 0.0, 1.0);
	float glowScale = clamp(time * blendSpeed, 1.0, 5.0);
	float glowIntensity = clamp(time * blendSpeed, 0.05, 5.0);
	float glowColorshift = clamp(time * blendSpeed, 0.75, 2.0);
	
	// pull in the textures
	vec4 texCube = texture(mainCubeTex, R);
	
	vec4 texDisplacement = texture(mainTex, IN.texCoord);
	vec4 texColour = texture(secondTex, IN.texCoord);
	vec4 texGrid = texture(thirdTex, IN.texCoord);
	
	// calculate how light effects the base texture
	vec3 ambient = texColour.rgb * lightColour * 0.1;
	vec3 diffuse = texColour.rgb * lightColour * lambert * atten;
	vec3 specular = lightColour * sFactor * atten;
	vec3 texColourWithLight = ambient + diffuse + specular;
	
	vec3 effectDirection = normalize(meshStart.xyz - meshEnd.xyz);
	float dBandFactor = 1.0f / dissolveBand;
	vec3 texPoint = mix
					(
						meshEnd.xyz - (dissolveBand * effectDirection)
						, meshStart.xyz
						, dissolveScale
					);
	float texCurrentPoint = dot(IN.worldPos - texPoint, effectDirection) * dBandFactor;

	// calculate our alpha values
	float texDissolve = (texDisplacement.r + (-2.0f * dissolveScale + 1.0f)) + texCurrentPoint;
	float alpha = clamp(texDissolve, 0.0f, 1.0f);
	
	// using glow scale factor predict the next spot on the texture
	float predictPos = (glowScale - texDissolve) * glowIntensity; // shift by intensity
	
	// calculate a new interpolated colour by adding a gradient
	float predictColour = clamp((glowScale * glowColorshift - texDissolve) * glowIntensity, 0.0f, 1.0f);
	vec4 glowCol = clamp 
					(
						predictPos * mix(glowSetCol, glowSetCol, predictColour)
						, 0.0f
						, 1.0f
					);

	// if the cube would be transparent, pull in our other texture so it looks like 
	// we are left with a wireframe
	if (alpha > 0.99)
	{
		fragColor = vec4(texCube.rgb + texColourWithLight + glowCol.rgb, alpha);
	}
	else 
	{
		fragColor = texGrid;
	}
}
