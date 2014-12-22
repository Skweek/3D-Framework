#version 430

in vec2 vUV;

uniform sampler2D Texture;
uniform vec4 Colour;

out vec4 Diffuse;

void main()
{
	// Check to make sure that the texture isnt just black
	vec4 Texel = texture(Texture, vUV.xy);
	if(Texel.xyz == vec3(0)) Texel = vec4(1);

	Diffuse = Texel * Colour;
}