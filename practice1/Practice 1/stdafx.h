// stdafx.h: ���������� ���� ��� ����������� ��������� ���������� ������
// ��� ���������� ������ ��� ����������� �������, ������� ����� ������������, ��
// �� ����� ����������
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <SDL2\SDL.h>
#include <GL\glew.h>

// ��������� ������������ main �� SDL_main,
// �.�. ���������� ���������� c SUBSYSTEM:CONSOLE
#ifdef _WIN32
#undef main
#endif

// TODO: ���������� ����� ������ �� �������������� ���������, ����������� ��� ���������
