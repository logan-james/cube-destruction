#version 400 core

layout (triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec3 tePatchDistance[3];
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;

void main()
{
	vec3 avg = (tePosition[0] + tePosition[1] + tePosition[2]) / 3;    
	gFacetNormal = normalize(avg);    
	
	gPatchDistance = tePatchDistance[0];    
	gTriDistance = vec3(1, 0, 0);    
	gl_Position = gl_in[0].gl_Position;    
	EmitVertex();    
	
	gPatchDistance = tePatchDistance[1];    
	gTriDistance = vec3(0, 1, 0);    
	gl_Position = gl_in[1].gl_Position;    
	EmitVertex();    
	
	gPatchDistance = tePatchDistance[2];    
	gTriDistance = vec3(0, 0, 1);    
	gl_Position = gl_in[2].gl_Position;    
	EmitVertex();    
	
	EndPrimitive();
}