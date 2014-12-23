#version 430

in mat4 shadowCoord;

uniform sampler2D diffuse;
uniform sampler2D light;
uniform sampler2D normals;

uniform vec2 screenSize;

out vec4 out_Colour;


void main()
{	
	vec2 UV = gl_FragCoord.xy / screenSize;

	vec4 text = texture(diffuse, UV);
	vec4 lightText = texture(light, UV);
	
	out_Colour = text * lightText;
}