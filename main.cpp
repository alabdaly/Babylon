#define NOMINMAX
#include <stdio.h>
#include <vector>
#include "Engine/Babylon.h"
class Game : public Application
{
public:
	void Start() override
	{

	}

	void Update(float dt) override
	{
		Renderer::Clear(0.2, 0.2, 0.2);
	}

	void OnExit() override
	{
	}
private:
};


int main()
{
	Application* App = new Game();
	App->Run(1280, 720, "Farm");
	return 0;
}