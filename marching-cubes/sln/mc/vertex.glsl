#version 330 core

uniform mat4 modelViewPerspective;

layout( location = 2 ) in vec4 vertex;
layout( location = 3 ) in vec4 color;

out vec4 frag_color;

void main()
{
	frag_color = color;

	gl_Position = modelViewPerspective * vertex;
}