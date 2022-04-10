#pragma once
#include <glm/glm.hpp>
#include "Input.h"
#include "Renderer.h"
#include "Application.h"
class UI
{
public:
	virtual void Update() = 0;
};

class Button : public UI
{
public:
	void Create(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		m_Pos = pos;
		m_Size = size;
		m_Color = color;
	}

	void Update() override
	{
		glm::vec2 Mouse = Input::MousePositionScreen();
		int sw, sh;
		glfwGetWindowSize(Application::Get()->GetWindow()->GetWidnow(), &sw, &sh);
		Mouse.x /= sw;
		Mouse.y /= sh;
		Mouse -= 0.5;
		Mouse *= 2;
		if (Mouse.x > m_Pos.x && Mouse.x < m_Pos.x + m_Size.x && Mouse.y > m_Pos.y && Mouse.y < m_Pos.y + m_Size.y)
		{
			if (Input::KeyPressed(1))
			{
				OnClick();
				goto smh;
			}
			OnHover();
		}
		smh:

		Renderer::DrawUI(m_Pos, m_Size, m_Color);
	}

	void (*OnClick)() = []() {};
	void (*OnHover)() = []() {};
private:
	glm::vec2 m_Pos, m_Size;
	glm::vec4 m_Color;
};

class InputField : public UI
{
public:
	InputField();
	void Update() override;
private:
	static InputField* Selected;
};
