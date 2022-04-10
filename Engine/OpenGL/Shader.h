#pragma once
#include <cinttypes>
#include "glad/glad.h"
#include "glm/glm.hpp"
class Shader
{
public:
	void Create(const char* path);
	void Create(const char* vertexSource, const char* fragmentSource);

	void Bind() { glUseProgram(m_ID); }
	void Unbind() { glUseProgram(0); };


	void UploadUniformIV1(const char* name, int* values, unsigned int count)
	{
		glUniform1iv(GetShaderLocation(name), count, values);
	}
	void UploadUniformI1(const char* name, int value)
	{
		glUniform1i(GetShaderLocation(name), value);
	}

	void UploadUniformF1(const char* name, float value)
	{
		glUniform1f(GetShaderLocation(name), value);
	}
	void UploadUniformF2(const char* name, float value1, float value2)
	{
		glUniform2f(GetShaderLocation(name), value1, value2);
	}
	void UploadUniformF3(const char* name, float value1, float value2, float value3)
	{
		glUniform3f(GetShaderLocation(name), value1, value2, value3);
	}
	void UploadUniformMat4(const char* name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetShaderLocation(name), 1, GL_FALSE, &mat[0][0]);
	}
private:
	int GetShaderLocation(const char* name)
	{
		int result = glGetUniformLocation(m_ID, name);
		if (result == -1)
		{
			printf("Could not find uniform: %s\n",name);
		}
		return result;
	}
private:
	uint32_t m_ID;
};