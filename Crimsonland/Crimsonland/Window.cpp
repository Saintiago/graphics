#include "stdafx.h"
#include "Window.h"
#include "Bodies.h"
#include "Decorators.h"
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace
{
const glm::vec4 BLACK = {0, 0, 0, 1};
const float MATERIAL_SHININESS = 30.f;
const glm::vec4 FADED_WHITE_RGBA = {0.3f, 0.3f, 0.3f, 1.f};
const glm::vec4 YELLOW_RGBA = {1, 1, 0, 1};
const glm::vec4 RED_RGBA = { 1, 0, 0, 1 };
const glm::vec4 BLUE_RGBA = { 0, 0, 1, 1 };
const glm::vec4 LIGHT_BLUE_RGBA = { 0.5f, 0.5f, 1, 1 };
const glm::vec4 DARK_BLUE_RGBA = { 0, 0, 0.5f, 1 };
const glm::vec4 GREEN_RGBA = { 0, 1, 0, 1 };
const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };
const glm::vec3 SUNLIGHT_DIRECTION = {-1.f, 0.2f, 0.7f};
const float CAMERA_INITIAL_ROTATION = 0;
const float CAMERA_INITIAL_DISTANCE = 5.f;

void CheckOpenglVersion()
{
	char *version = (char*)glGetString(GL_VERSION);
}

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);

    // включаем систему освещения
    glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

float GetSincFromXY(float x, float y)
{
    const float radius = std::hypotf(x, y);
    if (radius < std::numeric_limits<float>::epsilon())
    {
        return 1;
    }
    return sinf(radius) / radius;
}

glm::vec3 TransformPoint(const glm::vec3 point, const glm::mat4 &transform)
{
    const glm::vec4 original(point, 1);
    glm::vec4 transformed = transform * original;
    transformed /= transformed.w;

    return glm::vec3(transformed);
}


}

CWindow::CWindow()
    : m_camera(CAMERA_INITIAL_ROTATION, CAMERA_INITIAL_DISTANCE)
    , m_sunlight(GL_LIGHT0)
{
	m_player = std::make_unique<CPlayer>();
	m_arena = std::make_unique<CArena>(10, 10);

    SetBackgroundColor(BLACK);

    m_sunlight.SetDirection(SUNLIGHT_DIRECTION);
    m_sunlight.SetDiffuse(WHITE_RGBA);
    m_sunlight.SetAmbient(0.1f * WHITE_RGBA);
    m_sunlight.SetSpecular(WHITE_RGBA);
}

void CWindow::OnWindowInit(const glm::ivec2 &size)
{
    (void)size;
    SetupOpenGLState();
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
    m_camera.Update(deltaSeconds);

	m_player->Update(deltaSeconds);
}

void CWindow::OnDrawWindow(const glm::ivec2 &size)
{
    SetupView(size);
    m_sunlight.Setup();

	m_arena->Draw();
	m_player->Draw();
}

void CWindow::SetupView(const glm::ivec2 &size)
{
    glViewport(0, 0, size.x, size.y);

    // Матрица вида возвращается камерой и составляет
    // начальное значение матрицы GL_MODELVIEW.
    glLoadMatrixf(glm::value_ptr(m_camera.GetViewTransform()));

    // Матрица перспективного преобразования вычисляется функцией
    // glm::perspective, принимающей угол обзора, соотношение ширины
    // и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
    const float fieldOfView = glm::radians(70.f);
    const float aspect = float(size.x) / float(size.y);
    const float zNear = 0.01f;
    const float zFar = 100.f;
    const glm::mat4 proj = glm::perspective(fieldOfView, aspect, zNear, zFar);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);
}

glm::mat4 CWindow::GetProjectionMatrix(const glm::ivec2 &size)
{
	// Матрица перспективного преобразования вычисляется функцией
	// glm::perspective, принимающей угол обзора, соотношение ширины
	// и высоты окна, расстояния до ближней и дальней плоскостей отсечения.
	const float fieldOfView = glm::radians(70.f);
	const float aspect = float(size.x) / float(size.y);
	const float zNear = 0.01f;
	const float zFar = 100.f;

	return glm::perspective(fieldOfView, aspect, zNear, zFar);
}

void CWindow::OnKeyDown(const SDL_KeyboardEvent &event)
{
    m_player->OnKeyDown(event);
}

void CWindow::OnKeyUp(const SDL_KeyboardEvent &event)
{
	m_player->OnKeyUp(event);
}

void CWindow::OnMouseMotion(const glm::vec2 &pos)
{
	// Вычисляем позицию точки в нормализованных координатах окна,
	//  то есть на диапазоне [-1; 1].
	// Также переворачиваем координату "y",
	//  т.к. OpenGL считает нулевым нижний левый угол окна,
	//  а все оконные системы - верхний левый угол.
	const glm::ivec2 winSize = GetWindowSize();
	const glm::vec2 halfWinSize = 0.5f * glm::vec2(winSize);
	const glm::vec2 invertedPos(pos.x, winSize.y - pos.y);
	const glm::vec2 normalizedPos = (invertedPos - halfWinSize) / halfWinSize;

	// Вычисляем матрицу обратного преобразования
	//  поскольку поле игры не имеет своей трансформации,
	//  мы берём матрицу камеры в качестве ModelView-матрицы
	const glm::mat4 mvMat = m_camera.GetViewTransform();
	const glm::mat4 projMat = GetProjectionMatrix(winSize);
	const glm::mat4 inverse = glm::inverse(projMat * mvMat);

	// В нормализованном пространстве глубина изменяется от -1 до +1.
	// Вычисляем начало и конец отрезка, проходящего через
	//  нормализованное пространство насквозь.
	const glm::vec3 start = TransformPoint(glm::vec3(normalizedPos, -1.f), inverse);
	const glm::vec3 end = TransformPoint(glm::vec3(normalizedPos, +1.f), inverse);

	m_player->OnMouseMotion(CRay(start, end - start));
}
