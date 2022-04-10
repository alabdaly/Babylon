#pragma once
#include "glad/glad.h"
#include <cinttypes>
class VertexArray
{
public:
	void Create()
	{
		glGenVertexArrays(1, &m_ID);
	}

	void Bind()
	{
		glBindVertexArray(m_ID);
	}
	void Unbind()
	{
		glBindVertexArray(0);
	}
private:
	uint32_t m_ID;
};