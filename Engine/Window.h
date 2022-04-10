#pragma once
#include <cinttypes>
#include <string>
#include "GLFW/glfw3.h"
class Window
{
public:
	Window(uint32_t width, uint32_t height, const std::string& title) : m_Width(width), m_Height(height), m_Title(title)
	{
			if (!glfwInit())
			{
				printf("Failed To Initialize GLFW");
			}
			Create();
	}
	~Window()
	{
	}

	GLFWwindow* GetWidnow() { return m_Window; }

private:
	void Create()
	{
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);
		#ifdef _DEBUG
			if (!m_Window)
			{
				printf("Failed To Create GLFW Window");
			}
		#endif 
		glfwMakeContextCurrent(m_Window);
	}
private:
	uint32_t m_Width, m_Height;
	std::string m_Title;

	GLFWwindow* m_Window;
};