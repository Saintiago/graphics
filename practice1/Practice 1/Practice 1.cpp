// Practice 1.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{
	// ����������� �������� SDL_WINDOWPOS_CENTERED ��� x � y �������� SDL2
	// ���������� ���� � ������ �������� �� ���� x � y.
	// ����� ��� ������� ������������ 0, �.�. ���� �������� � ����� ������� ���� ������.
	// ��� ������������� OpenGL �� ������ ������� ���� SDL_WINDOW_OPENGL.
	SDL_Window *window = SDL_CreateWindow(
		"SDL2/OpenGL Demo", 0, 0, 640, 480, SDL_WINDOW_OPENGL);

	// ������ �������� OpenGL, ��������� � �����.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	// ������ �� ����� �������� ������� ���� ���������� OpenGL
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	// � ����� - ����� ������������� ����� � ���� �� ������
	SDL_GL_SwapWindow(window);

	// ����� ��������� ������ ���������, SDL_GLContext ������ ���� �����
	SDL_GL_DeleteContext(glcontext);

    return 0;
}

