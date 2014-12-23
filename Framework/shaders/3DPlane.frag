#version 430

in vec3 vPos;
in vec2 vUV;

uniform vec3 Normals;
uniform sampler2D textureMap;

out vec4 Diffuse;
out vec4 Normal;
out vec4 Position;

void main()
{
	Diffuse = texture(textureMap, vUV);
	if(Diffuse.a == 0) Diffuse = vec4(1,1,1,1);

	Normal = vec4(normalize(Normals),1);
	Position = vec4(vPos, 1);
}