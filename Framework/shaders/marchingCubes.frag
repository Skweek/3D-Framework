#version 330

in vec3 vNormal;
in vec4 vPos;

out vec4 Diffuse;
out vec4 Normal;
out vec4 Position;
out vec4 Specular;
out vec4 Material;

uniform vec4 materialColour;

void main()
{
	Diffuse = materialColour;
	Normal = vec4(vNormal, 1);
	Position = vPos;
	
	Specular = vec4(1,1,1,1);
	Material = vec4(1,1,1,1);
}