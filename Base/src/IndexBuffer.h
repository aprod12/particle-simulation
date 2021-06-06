#pragma once

class IndexBuffer
{

private:
	unsigned int m_RendererID;

public:
	IndexBuffer();
	~IndexBuffer();

	void Bind();
	void UnBind();


};
