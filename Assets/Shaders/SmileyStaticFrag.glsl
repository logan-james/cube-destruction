#version  400  core

uniform  sampler2D     mainTex;     //old  texture , on Tex  Unit 0
uniform  sampler2D     secondTex;   //new  texture , on Tex  Unit 1

uniform  float  time; //From  the  previous  tutorial!

in Vertex
{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void)
{
	vec4 smileyColour = texture(mainTex, IN.texCoord);
	
	if(smileyColour.b > 0.1)
	{
		vec2  tempTex = IN.texCoord;
		tempTex.y    += time;
		fragColour = texture(secondTex , tempTex );
	}
	else 
	{
		fragColour = smileyColour;
	}
}