#version 430

in vec2 vTexcoord;

out vec4 Diffuse;

uniform sampler2D tex;

void main() 
{
	vec4 col = texture(tex, vec2(vTexcoord.x, 1 - vTexcoord.y));
	if(col.a < 0.1) discard;

	Diffuse = texture(tex, vec2(vTexcoord.x, 1 - vTexcoord.y)) * vec4(1,0,0,1);
}