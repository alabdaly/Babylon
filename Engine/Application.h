#pragma once
#include "Window.h"
class Application
{
public:
	Application();
	~Application();
	void Run(unsigned int width, unsigned int height, const char* title);
	virtual void Start()
	{

	}

	virtual void Update(float dt)
	{

	}

	virtual void OnExit()
	{
		
	}



	static Application* Get() { return s_Instance; }
	Window* GetWindow() { return window; }
private:
	void OnExitUnderLay();
private:
	Window* window;
	static Application* s_Instance;
	bool b_Run = true;
};