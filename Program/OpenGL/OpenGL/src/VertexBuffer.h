#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* podaci, int velicina);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};