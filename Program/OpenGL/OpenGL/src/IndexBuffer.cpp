#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const void* podaci, unsigned int kolicina) : kolicina(kolicina)
{
	POTVRDA(sizeof(GLuint) == sizeof(unsigned int));

	GLProveraGresaka(glGenBuffers(1, &m_RendererID));
	GLProveraGresaka(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLProveraGresaka(glBufferData(GL_ELEMENT_ARRAY_BUFFER, kolicina * sizeof(unsigned int), podaci, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLProveraGresaka(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLProveraGresaka(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLProveraGresaka(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
