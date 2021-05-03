#shader vertex
#version 330 core

layout(location = 0) in vec4 pozicija;
layout(location = 1) in vec3 boja;

out vec3 fBoja;

uniform float u_ugao;

uniform mat4 u_MVP;

vec4 transliraj(vec4 meta, vec3 zaKoliko) {
	mat4 matrica = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-zaKoliko.x, -zaKoliko.y, -zaKoliko.z, 1
	);

	return matrica * meta;
}

void main()
{
	/*mat2 matrica = mat2(
		cos(u_ugao), -sin(u_ugao),
		sin(u_ugao), cos(u_ugao)
		);*/

	fBoja = boja;
	gl_Position = u_MVP * vec4(pozicija.xyz, 1);
};

#shader fragment
#version 330 core

in vec3 fBoja;

layout(location = 0) out vec4 boja;


void main()
{
	boja = vec4(fBoja, 1);
};