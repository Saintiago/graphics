#include "stdafx.h"
#include "WindowClient.h"

namespace
{
const glm::vec4 BLACK = {0, 0, 0, 1};
const float CAMERA_INITIAL_ROTATION = 0;
const float CAMERA_INITIAL_DISTANCE = 50;

void SetupOpenGLState()
{
    // включаем механизмы трёхмерного мира.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // включаем систему освещения
    glEnable(GL_LIGHTING);
}

glm::vec3 TransformPoint(const glm::vec3 point, const glm::mat4 &transform)
{
	const glm::vec4 original(point, 1);
	glm::vec4 transformed = transform * original;
	transformed /= transformed.w;

	return glm::vec3(transformed);
}

glm::mat4 GetProjectionMatrix(const glm::ivec2 &size)
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

}

CWindowClient::CWindowClient(CWindow &window)
    : CAbstractWindowClient(window)
    , m_camera(CAMERA_INITIAL_ROTATION, CAMERA_INITIAL_DISTANCE)
    , m_sunlight(GL_LIGHT0)
    , m_programFixed(CShaderProgram::fixed_pipeline_t())
{
    const glm::vec3 SUNLIGHT_DIRECTION = {-1.f, 0.2f, 0.7f};
    const glm::vec4 WHITE_RGBA = {1, 1, 1, 1};
    const glm::vec4 DARK_BLUE_RGBA = {0.2f, 0.2f, 0.6f, 1.f};
    const float AMBIENT_SCALE = 0.2f;

    window.SetBackgroundColor(BLACK);
    CheckOpenGLVersion();
    SetupOpenGLState();

    m_sunlight.SetDirection(SUNLIGHT_DIRECTION);
    m_sunlight.SetDiffuse(WHITE_RGBA);
    m_sunlight.SetAmbient(0.1f * WHITE_RGBA);
    m_sunlight.SetSpecular(WHITE_RGBA);

    const std::string vertexShader = CFilesystemUtils::LoadFileAsString("res/lambert-phong.vert");
    const std::string lambertShader = CFilesystemUtils::LoadFileAsString("res/lambert.frag");
    const std::string phongShader = CFilesystemUtils::LoadFileAsString("res/phong.frag");

    m_programLambert.CompileShader(vertexShader, ShaderType::Vertex);
    m_programLambert.CompileShader(lambertShader, ShaderType::Fragment);
    m_programLambert.Link();

    m_programPhong.CompileShader(vertexShader, ShaderType::Vertex);
    m_programPhong.CompileShader(phongShader, ShaderType::Fragment);
    m_programPhong.Link();

    m_programQueue = { &m_programPhong, &m_programLambert, &m_programFixed };

	m_player = std::make_unique<CPlayer>();
	m_arena = std::make_unique<CArena>(100, 100);
}

void CWindowClient::OnUpdateWindow(float deltaSeconds)
{
    m_camera.Update(deltaSeconds);
	m_player->Update(deltaSeconds);
}

void CWindowClient::OnDrawWindow()
{
    SetupView(GetWindow().GetWindowSize());

    m_sunlight.Setup();

    // Активной будет первая программа из очереди.
    m_programQueue.front()->Use();

	m_arena->Draw();
	m_player->Draw();
}

void CWindowClient::OnKeyDown(const SDL_KeyboardEvent &event)
{
	m_camera.OnKeyDown(event);
	m_player->OnKeyDown(event);
}

void CWindowClient::OnKeyUp(const SDL_KeyboardEvent &event)
{
	m_camera.OnKeyUp(event);
	m_player->OnKeyUp(event);
}

void CWindowClient::CheckOpenGLVersion()
{
    // В OpenGL 2.0 шейдерные программы вошли в спецификацию API.
    if (!GLEW_VERSION_2_0)
    {
        throw std::runtime_error("Sorry, but OpenGL 3.2 is not available");
    }
}

void CWindowClient::SetupView(const glm::ivec2 &size)
{
    glViewport(0, 0, size.x, size.y);
    const glm::mat4 mv = m_camera.GetViewTransform(m_player->GetPosition());
    glLoadMatrixf(glm::value_ptr(mv));

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

void CWindowClient::OnMouseMotion(const glm::vec2 &pos)
{
	// Вычисляем позицию точки в нормализованных координатах окна,
	//  то есть на диапазоне [-1; 1].
	// Также переворачиваем координату "y",
	//  т.к. OpenGL считает нулевым нижний левый угол окна,
	//  а все оконные системы - верхний левый угол.
	const glm::ivec2 winSize = GetWindow().GetWindowSize();
	const glm::vec2 halfWinSize = 0.5f * glm::vec2(winSize);
	const glm::vec2 invertedPos(pos.x, winSize.y - pos.y);
	const glm::vec2 normalizedPos = (invertedPos - halfWinSize) / halfWinSize;

	// Вычисляем матрицу обратного преобразования
	//  поскольку поле игры не имеет своей трансформации,
	//  мы берём матрицу камеры в качестве ModelView-матрицы
	const glm::mat4 mvMat = m_camera.GetViewTransform(m_player->GetPosition());
	const glm::mat4 projMat = GetProjectionMatrix(winSize);
	const glm::mat4 inverse = glm::inverse(projMat * mvMat);

	// В нормализованном пространстве глубина изменяется от -1 до +1.
	// Вычисляем начало и конец отрезка, проходящего через
	//  нормализованное пространство насквозь.
	const glm::vec3 start = TransformPoint(glm::vec3(normalizedPos, -1.f), inverse);
	const glm::vec3 end = TransformPoint(glm::vec3(normalizedPos, +1.f), inverse);

	m_player->OnMouseMotion(CRay(start, end - start));
}
