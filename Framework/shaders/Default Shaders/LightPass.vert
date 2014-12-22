#version 430

// Vert inputs
layout (location = 0)in vec4 Position;     

out vec4 vPos;
out vec4 vCol;
out vec3 vLightPos;
out vec3 vLightDir;
out mat4 shadowCoord;

uniform vec4 Colour;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform mat4 shadowMVP;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

const mat4 SHADOW_BIAS_MATRIX = mat4(	0.5, 0.0, 0.0, 0.0,
										0.0, 0.5, 0.0, 0.0,
										0.0, 0.0, 0.5, 0.0,
										0.5, 0.5, 0.5, 1.0	);

void main()
{
	vPos = Position;
	vCol = Colour;
	vLightPos = lightPos;
	vLightDir = lightDir;
	shadowCoord = SHADOW_BIAS_MATRIX * shadowMVP;

	gl_Position = Projection * View * Model *  vPos;
}