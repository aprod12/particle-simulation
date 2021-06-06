#pragma once
#include "VertexBuffer.h"


class VertexBufferLayout;

class VertexArray {

private:
	unsigned int m_RendererID;
	unsigned int m_VBSize;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
	unsigned int GetSize() const
	{
		return m_VBSize;
	}
};