#version 430

in vec3 position;
in vec2 texcoord;

out vec2 vTexcoord;

uniform mat4 Projection;
uniform mat4 Model;

void main() 
{
	vTexcoord = texcoord;
	gl_Position = Projection * Model * vec4(position,1);
}