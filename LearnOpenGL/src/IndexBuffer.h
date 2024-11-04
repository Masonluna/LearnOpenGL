#pragma once
#include "VertexLayout.h"
#include <vector>

class IndexBuffer
{
public:
	IndexBuffer() = default;
	IndexBuffer(std::vector<unsigned int> indices);
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
private:
	unsigned int m_RendererID = 0;
	unsigned int m_Count = 0;
};