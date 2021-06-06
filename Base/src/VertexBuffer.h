#pragma once

class VertexBuffer
{

private:
	unsigned int m_RendererID;
	unsigned int m_DataSize;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const ;
	void UnBind() const ;
	unsigned int GetDataSize() const;
};
