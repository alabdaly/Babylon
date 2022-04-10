#include "Input.h"
#include <Windows.h>
namespace Input
{
	short OldKeys[256];
	short Keys[256];
	glm::dvec2 pos;
	glm::dvec2 posworld;
	void Update(GLFWwindow* window)
	{
		
		for (int i = 0; i < 256; i++)
		{
			OldKeys[i] = Keys[i];
			Keys[i] = GetAsyncKeyState(i);
		}
		glfwGetCursorPos(window, &pos.x, &pos.y);

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		posworld.x = pos.x / width * 1600;
		posworld.y = pos.y / height * 900;
	}

	bool KeyHeld(int keycode)
	{
		return Keys[keycode];
	}

	bool KeyPressed(int keycode)
	{
		return Keys[keycode] && !OldKeys[keycode];
	}

	glm::dvec2& MousePosition()
	{
		
		return posworld;
	}
	glm::dvec2& MousePositionScreen()
	{
		return pos;
	}
}