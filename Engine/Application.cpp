#include "Application.h"
#include "Window.h"
#include "Renderer.h"
#include "Input.h"
Application* Application::s_Instance = nullptr;
Application::Application()
{
	s_Instance = this;
}

Application::~Application()
{

}

void ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void Application::Run(unsigned int width, unsigned int height, const char* title)
{
	window = new Window(width, height, title);
	Renderer::Initialize();
	glfwSetWindowSizeCallback(window->GetWidnow(), ResizeCallback);
	Start();

	const double maxFPS = 60.0;
	const double maxPeriod = 1.0 / maxFPS;

	double lastTime = 0.0;
	float dt = 0;
	while (b_Run)
	{
		double time = glfwGetTime();
		double deltaTime = time - lastTime;
		
		if (deltaTime >= maxPeriod) {
			lastTime = time;
			// code here gets called with max FPS
			Input::Update(window->GetWidnow());
			Renderer::BeginScene();
			Update(maxPeriod);
			Renderer::EndScene();
			/* Swap front and back buffers */
			glfwSwapBuffers(window->GetWidnow());

			/* Poll for and process events */
			glfwPollEvents();
		}
		
		


		

		//b_Run = glfwWindowShouldClose(window.GetWidnow());
	}
	OnExit();
}

void Application::OnExitUnderLay()
{
	OnExit();
	glfwTerminate();
	delete this;
}