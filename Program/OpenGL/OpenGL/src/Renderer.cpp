#include <iostream>

#include "Renderer.h"

void GLResetujGreske() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLIpisiGreske(const char* funkcija, const char* fajl, int linija) {
	bool nemaGreske = true;

	while (GLenum errorKod = glGetError())
	{
		std::cout << "[OPENGL GRESKA] KOD:(0x" << std::hex << (unsigned int)errorKod << std::dec << ") " << funkcija << "::'" << fajl << "'::" << linija << std::endl;
		nemaGreske = false;
	}

	return nemaGreske;
}