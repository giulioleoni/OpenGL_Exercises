#include <string>
#include <format>
#include "Window.h"
#include "Ex01TriangleDraw.h"
#include "Ex02QuadDraw.h"
#include "Ex03QuadIndexDraw.h"
#include "Ex04QuadColorDraw.h"
#include "Ex05QuadTextureDraw.h"
#include "Ex06QuadPrespDraw.h"
#include "Ex07CubePrespDraw.h"
#include "Ex08PhongDraw.h"
#include "Ex09InstancingDraw.h"
#include "Ex10Exercise.h"
#include "Ex11PostFX.h"
#include "Ex12DeferredDraw.h"

int main() {

	Window Window{800, 600, "Hello OpenGL"};

	/* PrintInfo()
	std::cout << "GL VERSION: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GL RENDERER: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "GL VENDOR: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "GLSL VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	*/

	float titleUpdateMaxTime = 1.f;
	float titleUpdateElapsed = 0.f;

	//Ex01TriangleDraw Scene;
	//Ex02QuadDraw Scene;
	//Ex03QuadIndexDraw Scene;
	//Ex04QuadColorDraw Scene;
	//Ex05QuadTextureDraw Scene;
	//Ex06QuadPrespDraw Scene;
	//Ex07CubePrespDraw Scene;
	//Ex08PhongDraw Scene;
	//Ex09InstancingDraw Scene;
	//Ex10Exercise Scene;
	//Ex11PostFX Scene;
	Ex12DeferredDraw Scene;
	
	Scene.Start();

	while(Window.IsOpened()) 
	{	
		float DeltaTime = Window.DeltaTime();
		titleUpdateElapsed += DeltaTime;
		if (titleUpdateElapsed >= titleUpdateMaxTime) //1.1 >= 1
		{
			int fps = (int)(1.f / DeltaTime);
			std::string formattedTitle = std::format("OpenGL App | DeltaTime: {} - FPS: {}", DeltaTime, fps);
			Window.SetTitle(formattedTitle);

			titleUpdateElapsed -= titleUpdateMaxTime; //0.1
		}

		Scene.Update(DeltaTime);

		Window.Update();
	}
	Scene.Destroy();
	return 0;	
}