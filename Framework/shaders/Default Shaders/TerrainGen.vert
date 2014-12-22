#version 430

// Vert inputs
layout (location = 0)in vec3 Position;     

out vec2 vPos;

void main()
{
	vPos = Position.xy;
	gl_Position = vec4(Position,1);
}