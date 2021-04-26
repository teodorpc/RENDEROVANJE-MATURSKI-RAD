#include <iostream>

#include<fstream>
#include<sstream>
#include<string>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

void UcitajShadere(std::string& vs, std::string& fs) {
	enum class tipoviShadera {
		NIKAKAV = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream("res/shaders/Basic.shader");

	std::string line;

	tipoviShadera tip = tipoviShadera::NIKAKAV;

	std::stringstream strs[2];

	while (getline(stream, line)) 
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				tip = tipoviShadera::VERTEX;
			else if (line.find("#shader") != std::string::npos)
				tip = tipoviShadera::FRAGMENT;
		}
		else
			strs[(int)tip] << line << '\n';

#ifdef DEBUG
	std::cout << "VERTEX" << std::endl;
	std::cout << strs[0].str() << std::endl;
	std::cout << "FRAGMENT" << std::endl;
	std::cout << strs[1].str() << std::endl;
#endif

	vs = strs[0].str();
	fs = strs[1].str();
}

unsigned int KompajlujShader(unsigned int tip, std::string src) {
	unsigned int shader = glCreateShader(tip);
	const char* str = src.c_str();
	glShaderSource(shader, 1, &str, 0);
	glCompileShader(shader);
	
	int rezultat;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &rezultat);
	if (rezultat == GL_FALSE) 
	{
		int duzina;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &duzina);
		char* poruka = (char*)_malloca(duzina * sizeof(char));
		glGetShaderInfoLog(shader, duzina * sizeof(char), &duzina, poruka);
		std::cout << (tip == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader je neuspesno kompajlovan." << std::endl;
		std::cout << poruka << std::endl;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

unsigned int StvoriShadere(std::string VertexShaderSource, std::string FragmentShaderSource) {
	unsigned int program = glCreateProgram();
	unsigned int vs = KompajlujShader(GL_VERTEX_SHADER, VertexShaderSource);
	unsigned int fs = KompajlujShader(GL_FRAGMENT_SHADER, FragmentShaderSource);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;



	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "GRESKA" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	float pozicije[8] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	int indexi[6] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vertexArrayIndex;
	GLProveraGresaka(glGenVertexArrays(1, &vertexArrayIndex));
	GLProveraGresaka(glBindVertexArray(vertexArrayIndex));

	VertexBuffer vb(pozicije, 8 * sizeof(float));

	IndexBuffer ib(indexi, 6);


	GLProveraGresaka(glEnableVertexAttribArray(0));
	GLProveraGresaka(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	std::string vs = "#version 330 core\n"
		"layout(location = 0) in vec4 pozicija;\n"
		"void main()\n"
		"{\n"
		"gl_Position = pozicija;\n"
		"}\n";
	std::string fs = "#version 330 core\n"
		"layout(location = 0) out vec4 boja;\n"
		"void main()\n"
		"{\n"
		"boja = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	GLProveraGresaka(UcitajShadere(vs, fs));

	GLProveraGresaka(unsigned int program = StvoriShadere(vs, fs));
	GLProveraGresaka(glUseProgram(program));
	
	GLProveraGresaka(int index_u_Boja = glGetUniformLocation(program, "u_boja"));

	POTVRDA(index_u_Boja != -1);

	glUniform4f(index_u_Boja, 0.0f, 0.0f, 1.0f, 1.0f);

	float dim = 0.0f;

	float smer = 1.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		if (dim > 1.0f) {
			dim = 1.0f;
			smer = -1.0f;
		}
		else if (dim < 0.0f) {
			dim = 0.0f;
			smer = 1.0f;
		}

		GLProveraGresaka(glUniform4f(index_u_Boja, 0.2f * dim, 0.7f * dim, 0.2f * dim, 1.0f));

		GLProveraGresaka(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		dim += 0.01f * smer;


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLProveraGresaka(glDeleteProgram(program));

	glfwTerminate();
	return 0;
}