#version 400 core

in vec3 gFacetNormal;
in vec3 gPatchDistance;
in vec3 gTriDistance;
out vec4 out_Color;

float edge(float d, float scale, float offset)
{    
	d = scale * d + offset;    
	d = clamp(d, 0, 1);    
	d = 1 - exp2(-2*d*d);    
	return d;
}

void main()
{    
	float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);    
	float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);    
	vec3 color = edge(d1, 40, -0.5) * edge(d2, 60, -0.5) * vec3(0.5, 0.5, 0.5);    
	out_Color = vec4(color, 1.0);
}