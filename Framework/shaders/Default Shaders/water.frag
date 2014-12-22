#version 430

in vec4 vPos;
in vec3 normals;
in vec2 TexCoord;
in float off;

// water texture
uniform sampler2D textureMap;

out vec4 Diffuse;
out vec4 Normal;
out vec4 Position;

void main()
{
	if(off > 0.2)
		discard;

	// Get the noise value to determine what the colour should be
	Diffuse = vec4(texture(textureMap, TexCoord).rgb, 0.7);


	Normal = vec4(normalize(normals),1);
	Position = vPos;
}