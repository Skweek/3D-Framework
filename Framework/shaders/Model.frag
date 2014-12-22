#version 430

// frag inputs
in vec2 vUV;
in vec3 vNormal;
in vec4 vPos;

// texture
uniform sampler2D DiffuseTexture;
uniform vec4 lightSpecular;
uniform vec4 lightDiffuse;

uniform sampler2D sceneDepth;
uniform vec2 screenSize;

out vec4 Diffuse;
out vec4 Normal;
out vec4 Position;

out vec4 Material;
out vec4 Specular;

float LinearizeDepth(float zoverw)
{
	float n = 0.1; // camera z near
	float f = 100000.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}

void main()
{
	if(gl_FragCoord.z < LinearizeDepth(texture(sceneDepth, gl_FragCoord.xy / screenSize)))
		discard;
	
	vec4 col = texture(DiffuseTexture, vUV.xy);
	if(col.xyz == vec3(0)) col = lightDiffuse;
	if(col.a < 0.9) discard;


	Diffuse = col;
	Normal = vec4(vNormal,1) + 1 * 0.5;
	Position = vPos;
	
	Material = lightDiffuse;
	Specular = lightSpecular;
}