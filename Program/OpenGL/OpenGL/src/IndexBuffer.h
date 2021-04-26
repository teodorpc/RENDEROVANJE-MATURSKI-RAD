#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int kolicina;
public:
	IndexBuffer(const void* podaci, unsigned int kolicina);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};