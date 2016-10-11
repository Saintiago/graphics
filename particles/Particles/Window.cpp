#include "stdafx.h"
#include "Window.h"
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <time.h>

namespace
{
	const glm::vec4 QUIET_GREEN = { 0.f, 0.5f, 0.2f, 1.f };
	const glm::vec4 BLACK = { 0, 0, 0, 0};
}

CWindow::CWindow()
{
	m_particles = std::make_unique<CParticleSystem>();
	m_particles->Populate(30);
	SetBackgroundColor(BLACK);
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
	m_particles->Update(deltaSeconds);
}

void CWindow::OnDrawWindow(const glm::ivec2 &size)
{
	SetupView(size);
	m_particles->Draw();
}

void CWindow::OnDragBegin(const glm::vec2 &pos)
{
}

void CWindow::OnDragMotion(const glm::vec2 &pos)
{
}

void CWindow::OnDragEnd(const glm::vec2 &pos)
{
}

void CWindow::OnMouseUp(const SDL_MouseButtonEvent & event)
{
	const glm::vec2 clickCoord = MapMousePosition({ event.x, event.y });

	if (m_particles->DeleteParticleByCoords(clickCoord))
	{
		return;
	}

	if (event.button == SDL_BUTTON_LEFT)
	{
		m_particles->SpawnParticle(clickCoord, -1.f);
	}

	if (event.button == SDL_BUTTON_RIGHT)
	{
		m_particles->SpawnParticle(clickCoord, 1.f);
	}
}

void CWindow::OnKeyUp(const SDL_KeyboardEvent & event)
{
	if (event.keysym.scancode == SDL_SCANCODE_DELETE)
	{
		m_particles->RemoveAllParticles();
	}
}

void CWindow::SetupView(const glm::ivec2 &size)
{
	// Матрица ортографического проецирования изображения в трёхмерном пространстве
	// из параллелипипеда с размером, равным (size.X x size.Y x 2).
	const glm::mat4 matrix = glm::ortho<float>(0, float(VIRTUAL_FIELD_SIZE.x), float(VIRTUAL_FIELD_SIZE.y), 0);
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(matrix));
	glMatrixMode(GL_MODELVIEW);
}

glm::vec2 CWindow::MapMousePosition(glm::vec2 windowCoord) const
{
	const glm::vec2 windowSize = GetWindowSize();
	return VIRTUAL_FIELD_SIZE / (windowSize / windowCoord);
}
