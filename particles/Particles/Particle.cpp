#include "stdafx.h"
#include "Particle.h"
#include "ColoredShape.h"

const float CParticle::pointSize = 20;

CParticle::CParticle(glm::vec2 pos, sign sign)
	: m_pos(pos)
	, m_sign(sign)
{
	switch (sign)
	{
	case POSITIVE:
		SetColor(RED);
		break;
	case NEGATIVE:
		SetColor(BLUE);
		break;
	default:
		break;
	}
}


CParticle::~CParticle()
{
}

sign CParticle::GetSign()
{
	return m_sign;
}

glm::vec2 CParticle::GetPosition()
{
	return m_pos;
}

void CParticle::Redraw()const
{
	glPointSize(pointSize);
	glColor3f(m_color.x, m_color.y, m_color.z);
	glBegin(GL_POINTS);
	glVertex2f(m_pos.x, m_pos.y);
	glEnd();
}

