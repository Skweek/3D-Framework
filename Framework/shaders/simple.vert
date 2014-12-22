#version 430

// Vert inputs
layout (location = 0)in vec3 Position;     
layout (location = 1)in vec2 UV;     

out vec2 vUV;

uniform mat4 Model;

void main()
{
	vUV = UV;
	gl_Position = Model * vec4(Position,1);
}