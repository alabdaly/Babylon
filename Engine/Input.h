#pragma once
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
namespace Input
{
	void Update(GLFWwindow* window);
	bool KeyHeld(int keycode);
	bool KeyPressed(int keycode);

	glm::dvec2& MousePosition();

	glm::dvec2& MousePositionScreen();
}