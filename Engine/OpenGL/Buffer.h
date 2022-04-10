#pragma once
#include "glad/glad.h"
#include <cinttypes>

class VertexBuffer
{
public:
	void Create()
	{
		glGenBuffers(1, &m_ID);
	}

	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}
	void Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void UploadData(void* data, size_t size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
private:
	uint32_t m_ID;
};

class IndexBuffer
{
public:
	void Create()
	{
		glGenBuffers(1, &m_ID);
	}

	void Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
	void Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void UploadData(uint32_t* data, uint32_t count)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}
private:
	uint32_t m_ID;
};