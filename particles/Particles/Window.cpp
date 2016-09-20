#include "stdafx.h"
#include "Window.h"
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/adaptor/reversed.hpp>

namespace
{
	const glm::vec4 QUIET_GREEN = { 0.f, 0.5f, 0.2f, 1.f };
	const glm::vec4 BLACK = { 0, 0, 0, 0};
}

CWindow::CWindow()
{
	glm::vec2 staticPos = { 400, 300 }; // make it dynamic
	auto pParticle = std::make_unique<CParticle>(staticPos, NEGATIVE);
	m_particles.push_back(std::move(pParticle));
	SetBackgroundColor(BLACK);
}

void CWindow::OnUpdateWindow(float deltaSeconds)
{
	(void)deltaSeconds;
}

void CWindow::OnDrawWindow(const glm::ivec2 &size)
{
	SetupView(size);
	
	for (const auto &pParticle : m_particles)
	{
		pParticle->Draw();
	}
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

void CWindow::SetupView(const glm::ivec2 &size)
{
	// Матрица ортографического проецирования изображения в трёхмерном пространстве
	// из параллелипипеда с размером, равным (size.X x size.Y x 2).
	const glm::mat4 matrix = glm::ortho<float>(0, float(size.x), float(size.y), 0);
	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(matrix));
	glMatrixMode(GL_MODELVIEW);
}
