#version 430

// Vert inputs
layout (location = 0)in vec4 Position;     
layout (location = 1)in vec4 Color;
layout (location = 2)in vec2 TexCoord1;	
layout (location = 3)in vec4 Normal;
layout( location = 4 ) in vec4 indices;
layout( location = 5 ) in vec4 weights;

// skeleton animation
const int MaxBones = 128;
uniform mat4 bones[MaxBones];

// frag outputs
out vec3 vNormal;
out vec4 vPos;
out vec2 vUV;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	ivec4 index = ivec4(indices);
	vec4 P = Position;
	vec4 N = Normal;
	
	// sample bones and blend up to 4
	if (weights.x > 0 || weights.y > 0 || weights.z > 0 || weights.w > 0)
	{
		P = bones[ index.x ] * Position * weights.x;
		P += bones[ index.y ] * Position * weights.y;
		P += bones[ index.z ] * Position * weights.z;
		P += bones[ index.w ] * Position * weights.w;
					
		N *= transpose(bones[ index.x ]);
		N *= transpose(bones[ index.y ]);
		N *= transpose(bones[ index.z ]);
		N *= transpose(bones[ index.w ]);
	}
	vPos = P;
	vNormal = (transpose(inverse(Model)) * N).xyz;
	vUV = TexCoord1;

	gl_Position = Projection * View * Model * P;
}