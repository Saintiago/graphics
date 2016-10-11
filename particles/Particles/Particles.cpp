#include "stdafx.h"
#include "Window.h"
#include <SDL2/SDL.h>

int main(int, char *[])
{
	srand(static_cast<unsigned>(time(NULL)));

	glewInit();

	CWindow window;
	window.Show({ 600, 600 });
	window.DoGameLoop();

	return 0;
}
