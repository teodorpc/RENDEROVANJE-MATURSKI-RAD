#pragma once

#include <GL/glew.h>

#define POTVRDA(x) if(!(x)) __debugbreak()

#ifdef DEBUG
#define GLProveraGresaka(x) GLResetujGreske();\
							x;\
							if(!GLIpisiGreske(#x, __FILE__, __LINE__)) __debugbreak()
#else
#define GLProveraGresaka(x) x
#endif

void GLResetujGreske();

bool GLIpisiGreske(const char* funkcija, const char* fajl, int linija);