#version 330

layout (location = 0) in vec4 Position;

out vec4 vPosition;
out vec4 shadowCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 lightProjView;

const mat4 SHADOW_BIAS_MATRIX = mat4( 0.5, 0.0, 0.0, 0.0,
									  0.0, 0.5, 0.0, 0.0,
									  0.0, 0.0, 0.5, 0.0,
									  0.5, 0.5, 0.5, 1.0 );

void main()
{

	vPosition = Position;
	gl_Position = Projection * View * Model * Position;

	shadowCoord = SHADOW_BIAS_MATRIX * lightProjView * Position;
}