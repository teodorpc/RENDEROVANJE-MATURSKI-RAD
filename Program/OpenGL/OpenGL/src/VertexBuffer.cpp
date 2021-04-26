#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* podaci, int velicina)
{
	GLProveraGresaka(glGenBuffers(1, &m_RendererID));
	GLProveraGresaka(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLProveraGresaka(glBufferData(GL_ARRAY_BUFFER, velicina, podaci, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLProveraGresaka(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLProveraGresaka(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLProveraGresaka(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
