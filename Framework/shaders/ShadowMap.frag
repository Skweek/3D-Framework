#version 430

in vec2 vUV;

uniform sampler2D diffuse;

out float out_Colour;

void main()
{
	if(texture(diffuse, vUV).a < 0.5)
		discard;

	out_Colour = gl_FragCoord.z;
}