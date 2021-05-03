#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include<fstream>
#include<sstream>
#include<string>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

#include <vec3.hpp> // glm::vec3
#include <vec4.hpp> // glm::vec4
#include <mat4x4.hpp> // glm::mat4
#include <ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <ext/matrix_clip_space.hpp> // glm::perspective

const int visina = 720;
const int sirina = 720;

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

std::ostream& operator<<(std::ostream& os, const glm::vec3& dt) {
	return os << dt.x << " " << dt.y << " " << dt.z;
}

void podesiMVPMatricu(const glm::mat4 matricaModela, const glm::mat4 matricaKamere, const glm::mat4 matricaProjekcije, const int program) {
	glm::mat4 MVP = matricaProjekcije * matricaKamere * matricaModela;

	GLProveraGresaka(int index_u_MVP = glGetUniformLocation(program, "u_MVP"));
	POTVRDA(index_u_MVP != -1);
	GLProveraGresaka(glUniformMatrix4fv(index_u_MVP, 1, GL_FALSE, &MVP[0][0]));
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;



	/* Create a windowed mode window and its OpenGL context */

	window = glfwCreateWindow(sirina, visina, "Aplikacija", NULL, NULL);
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

	const int brojTemena = 8;
	const int brojTrouglova = 12;

	float pozicije[brojTemena * 3] = {
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f
	};

	float boje[brojTrouglova * 9];

	srand(time(0));

	

	for (int i = 0; i < brojTrouglova * 3; i++) {
		boje[3 * i + 0] = (float)rand() / (float)RAND_MAX;
		boje[3 * i + 1] = (float)rand() / (float)RAND_MAX;
		boje[3 * i + 2] = (float)rand() / (float)RAND_MAX;
	}

	int indexi[brojTrouglova * 3] = {
		0, 1, 5,
		0, 5, 4,
		0, 3, 1,
		0, 2, 3,
		2, 0, 4,
		2, 4, 6,
		1, 3, 7,
		1, 7, 5,
		3, 2, 6,
		3, 6, 7,
		4, 5, 7,
		4, 7, 6,
	};

	unsigned int vertexArrayIndex;
	GLProveraGresaka(glGenVertexArrays(1, &vertexArrayIndex));
	GLProveraGresaka(glBindVertexArray(vertexArrayIndex));

	VertexBuffer vb(pozicije, brojTemena * 3 * sizeof(float));

	IndexBuffer ib(indexi, brojTrouglova * 3);


	GLProveraGresaka(glEnableVertexAttribArray(0));
	GLProveraGresaka(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0));

	unsigned int bufferBojaIndex;
	glGenBuffers(1, &bufferBojaIndex);
	glBindBuffer(GL_ARRAY_BUFFER, bufferBojaIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boje), boje, GL_STATIC_DRAW);

	GLProveraGresaka(glEnableVertexAttribArray(1));
	GLProveraGresaka(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0));

	std::string vs;
	std::string fs;

	GLProveraGresaka(UcitajShadere(vs, fs));

	GLProveraGresaka(unsigned int program = StvoriShadere(vs, fs));
	GLProveraGresaka(glUseProgram(program));

	
	/*GLProveraGresaka(int index_u_Boja = glGetUniformLocation(program, "u_boja"));
	POTVRDA(index_u_Boja != -1);
	GLProveraGresaka(glUniform4f(index_u_Boja, 0.0f, 0.0f, 1.0f, 1.0f));*/

	float dim = 1.0f;

	float smer = 1.0f;

	float ugao = 0.0f;

	float ugaoX = 2.3f, ugaoY = 5.5f;


	glm::vec3 gledatiKaTacciNaSferi(cos(ugaoX) * cos(ugaoY), sin(ugaoY), sin(ugaoX) * cos(ugaoY));

	glm::mat4 matricaProjekcije = glm::perspective<float>(glm::radians(45.0f), (float)sirina / (float)visina, 0.1f, 100.0f);
	glm::vec3 pozicijaKamere(3, 4, -3);
	glm::mat4 matricaKamere = glm::lookAt(pozicijaKamere, pozicijaKamere + gledatiKaTacciNaSferi, glm::vec3(0, 1, 0));
	glm::mat4 matricaModela(1.0f);

	podesiMVPMatricu(matricaModela, matricaKamere, matricaProjekcije, program);

	float const1, const2, const3;

	float modifZaKretanje = 0.1f;
	float modifZaOkretanje = 0.035f;



	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0, 0);

	bool nijeBlokiran = true;

	glEnable(GL_CULL_FACE);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int modKursora = glfwGetInputMode(window, GLFW_CURSOR);

		int stanjeEsc = glfwGetKey(window, GLFW_KEY_ESCAPE);

		if (stanjeEsc == GLFW_PRESS && nijeBlokiran) {
			nijeBlokiran = false;
			if (modKursora == GLFW_CURSOR_DISABLED) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			glfwSetCursorPos(window, 0, 0);
		}
		else if(stanjeEsc == GLFW_RELEASE)
			nijeBlokiran = true;

		if (modKursora == GLFW_CURSOR_DISABLED) {
			double misX, misY;

			glfwGetCursorPos(window, &misX, &misY);

			ugaoX += misX / 1000;
			ugaoY -= misY / 1000;
			glfwSetCursorPos(window, 0, 0);
		}

		int stanjeStreliceDesno = glfwGetKey(window, GLFW_KEY_RIGHT);
		int stanjeStreliceLevo = glfwGetKey(window, GLFW_KEY_LEFT);

		int stanjeStreliceGore = glfwGetKey(window, GLFW_KEY_UP);
		int stanjeStreliceDole = glfwGetKey(window, GLFW_KEY_DOWN);

		if(stanjeStreliceDesno == GLFW_PRESS || stanjeStreliceDesno == GLFW_REPEAT)
			ugaoX += modifZaOkretanje;

		if (stanjeStreliceLevo == GLFW_PRESS || stanjeStreliceLevo == GLFW_REPEAT)
			ugaoX -= modifZaOkretanje;

		if (stanjeStreliceGore == GLFW_PRESS || stanjeStreliceGore == GLFW_REPEAT)
			ugaoY += modifZaOkretanje;

		if (stanjeStreliceDole == GLFW_PRESS || stanjeStreliceDole == GLFW_REPEAT)
			ugaoY -= modifZaOkretanje;

		int stanjeW = glfwGetKey(window, GLFW_KEY_W);
		int stanjeS = glfwGetKey(window, GLFW_KEY_S);

		int stanjeA = glfwGetKey(window, GLFW_KEY_A);
		int stanjeD = glfwGetKey(window, GLFW_KEY_D);

		if (ugaoX < 0)
			ugaoX += 2 * glm::pi<float>();
		else if (ugaoX > 2 * glm::pi<float>())
			ugaoX -= 2 * glm::pi<float>();

		if (ugaoY < 0)
			ugaoY += 2 * glm::pi<float>();
		else if (ugaoY > 2 * glm::pi<float>())
			ugaoY -= 2 * glm::pi<float>();

		glm::vec3 gledatiKaTacciNaSferi(cos(ugaoX) * cos(ugaoY), sin(ugaoY), sin(ugaoX) * cos(ugaoY));

		if (stanjeW == GLFW_PRESS || stanjeW == GLFW_REPEAT)
			pozicijaKamere += modifZaKretanje * gledatiKaTacciNaSferi;

		if (stanjeS == GLFW_PRESS || stanjeS == GLFW_REPEAT)
			pozicijaKamere -= modifZaKretanje * gledatiKaTacciNaSferi;

		if (stanjeA == GLFW_PRESS || stanjeA == GLFW_REPEAT)
			pozicijaKamere -= modifZaKretanje * glm::vec3(-sin(ugaoX), 0, cos(ugaoX));

		if (stanjeD == GLFW_PRESS || stanjeD == GLFW_REPEAT)
			pozicijaKamere += modifZaKretanje * glm::vec3(-sin(ugaoX), 0, cos(ugaoX));

		std::cout << "KOORDINATE NA KOJIMA SE NALAZITE SU X: " << pozicijaKamere.x << ", Y: " << pozicijaKamere.y << ", Z: " << pozicijaKamere.z << ", UGLOVI PO X I PO Y OSI U RADIJANIMA SU UGAOX: " << ugaoX << ", UGAOY: " << ugaoY << std::endl;

		glm::mat4 matricaKamere = glm::lookAt(pozicijaKamere, pozicijaKamere + gledatiKaTacciNaSferi, glm::vec3(0, 1, 0));
		podesiMVPMatricu(matricaModela, matricaKamere, matricaProjekcije, program);

		GLProveraGresaka(glDrawElements(GL_TRIANGLES, brojTrouglova * 3, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLProveraGresaka(glDeleteProgram(program));

	glfwTerminate();
	return 0;
}