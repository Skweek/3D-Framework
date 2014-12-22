#version 430

// Vert inputs
layout (location = 0)in vec3 Position;     
layout (location = 0)in vec2 UV;     

out vec3 vPos;
out vec2 vUV;

uniform mat4 ProjectionView;

void main()
{
	vPos = Position;
	vUV = UV;

	gl_Position = ProjectionView * vec4(Position,1);
}