#pragma once
#include "VertexLayout.h"
#include <vector>

class VertexBuffer
{
private:
	unsigned int m_RendererID = 0;
public:
	VertexBuffer() = default;
	VertexBuffer(std::vector<Vertex> vertices);
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};