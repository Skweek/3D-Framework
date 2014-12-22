#version 330

layout( location = 0 ) in vec3 position;

out vec3 Position;

void main()
{
	Position = position;
}