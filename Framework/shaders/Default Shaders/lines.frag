#version 430

in vec3 vPos;

out vec4 Diffuse;
out vec4 Position;

void main()
{
	Diffuse = vec4(1,0,0,1);
	Position = vec4(vPos, 1);
}