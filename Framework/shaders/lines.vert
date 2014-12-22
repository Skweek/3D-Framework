#version 430

// Vert inputs
layout (location = 0)in vec3 Position;     

out vec3 vPos;

uniform mat4 ProjectionView;

void main()
{
	vPos = Position;
	gl_Position = ProjectionView * vec4(Position,1);
}