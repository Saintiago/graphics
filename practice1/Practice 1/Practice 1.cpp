// Practice 1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	// Специальное значение SDL_WINDOWPOS_CENTERED для x и y заставит SDL2
	// разместить окно в центре монитора по осям x и y.
	// Здесь для примера используется 0, т.е. окно появится в левом верхнем углу экрана.
	// Для использования OpenGL вы ДОЛЖНЫ указать флаг SDL_WINDOW_OPENGL.
	SDL_Window *window = SDL_CreateWindow(
		"SDL2/OpenGL Demo", 0, 0, 640, 480, SDL_WINDOW_OPENGL);

	// Создаём контекст OpenGL, связанный с окном.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	// Теперь мы можем очистить область окна средствами OpenGL
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	// В конце - вывод нарисованного кадра в окно на экране
	SDL_GL_SwapWindow(window);

	// После окончания работы программы, SDL_GLContext должен быть удалён
	SDL_GL_DeleteContext(glcontext);

    return 0;
}

