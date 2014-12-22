#version 430

// Vert inputs
layout (location = 0)in vec4 Position;    
layout (location = 2)in vec2 UV;    

uniform mat4 MVP;

out vec2 vUV;

void main()
{
	vUV = UV;
	gl_Position = MVP * Position;
}