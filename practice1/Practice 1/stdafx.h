// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <SDL2\SDL.h>
#include <GL\glew.h>

// Выключаем макроподмену main на SDL_main,
// т.к. приложение собирается c SUBSYSTEM:CONSOLE
#ifdef _WIN32
#undef main
#endif

// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
