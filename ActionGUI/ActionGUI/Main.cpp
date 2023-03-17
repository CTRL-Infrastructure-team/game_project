# include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include"Display.h"
#include"Switch.h"

void Main()
{
	// 背景の色を設定 | Set background color
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
	Window::Resize(1000, 700);

	JSON json;
	Array<JSON> arrayjson;
	Display display;
	
	while (System::Update())
	{
		ClearPrint();
		display.update();
		display.draw();
	}
}
