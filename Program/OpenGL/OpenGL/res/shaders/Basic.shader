#shader vertex
#version 330 core

layout(location = 0) in vec4 pozicija;

void main()
{
	gl_Position = pozicija;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 boja;

uniform vec4 u_boja;

void main()
{
	boja = u_boja;
};