#include "stdafx.h"
#include "Parabola.h"

namespace
{
	const float GRAPH_PRECISION = 0.1;
}

static float CalculateFunction(float x)
{
	return (2 * x * x - 3 * x - 8);
}

CParabola::CParabola()
{
}


CParabola::~CParabola()
{
}

void CParabola::SetXAxisSpan(const glm::vec2 &spanX)
{
	m_spanX = spanX;
}

void CParabola::Redraw()const
{
	glBegin(GL_LINE_STRIP);

	glColor3f(m_color.x, m_color.y, m_color.z);

	for (float i = m_spanX.x; i <= m_spanX.y; i += GRAPH_PRECISION)
	{
		glm::vec2 pos = GetPosition(i);
		glVertex2f(pos.x, pos.y);
	}

	glEnd();
}

glm::vec2 CParabola::GetPosition(float x)const
{
	const float posX = m_windowCenter.x + (x * m_step.x);
	const float posY = m_windowCenter.y + (CalculateFunction(x) * m_step.y);
	return { posX, posY };
}