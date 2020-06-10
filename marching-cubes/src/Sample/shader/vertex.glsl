#version 330 core

uniform mat4 modelViewPerspective;

layout( location = 0 ) in vec3 vertex;
layout( location = 1 ) in vec3 color;

out vec3 frag_color;

void main()
{
	frag_color = color;

	gl_Position = modelViewPerspective * vec4( vertex, 1.0 );
}