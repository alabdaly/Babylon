#include "Shader.h"
#include <iostream>
#include <sstream>
void Shader::Create(const char* path)
{
	bool currentShader = 0; // 0 for Vertex, 1 for Fragment
	// read the files
	FILE* shader = fopen(path, "r");

	std::stringstream vertex;
	std::stringstream fragment;
	if (shader)
	{
		char buffer[512];
		while (fgets(buffer, 512, shader))
		{
			if (buffer[0] == '#')
			{
				if (buffer[1] == 'V')
				{
					currentShader = 0;
					continue;
				}
				else if (buffer[1] == 'F')
				{
					currentShader = 1;
					continue;
				}
			}

			if (currentShader)
			{
				fragment << buffer;
			}
			else
			{
				vertex << buffer;
			}
		}


		fclose(shader);

		Create(vertex.str().c_str(), fragment.str().c_str());
	}
	else
	{
		printf("Couldn't find shader: %s", path);
	}
}
void Shader::Create(const char* vertexSource, const char* fragmentSource)
{
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// similiar for Fragment Shader

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// shader Program
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	// print linking errors if any
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}