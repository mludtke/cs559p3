#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec3 vertex_normal;

out vec3 Position;
out vec3 Normal;

uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 projection_matrix;
uniform mat4 mvp;

void main()
{  
	Normal = normalize( normal_matrix * vertex_normal);
	Position = vec3( modelview_matrix * vec4(vertex_position,1.0));
	gl_Position = mvp * vec4(vertex_position,1.0);
}