#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 Position[];

out vec3 vNormal;
out vec4 vPos;

uniform mat4 projectionViewModel;
uniform mat4 transposeInvModel;

void main()
{
	vNormal = (transposeInvModel * vec4(normalize(cross(Position[2] - Position[1],Position[0] - Position[1])),0)).xyz;
	vPos = vec4(Position[0],1);

	gl_Position = projectionViewModel * vec4(Position[0],1);
	EmitVertex();
	
	gl_Position = projectionViewModel * vec4(Position[1],1);
	EmitVertex();

	gl_Position = projectionViewModel * vec4(Position[2],1);
	EmitVertex();
}