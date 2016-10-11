#include "stdafx.h"
#include "Particle.h"
#include "ColoredShape.h"

CParticle::CParticle(glm::vec2 pos, float charge)
	: m_pos(pos)
	, m_charge(charge)
{
	if (m_charge > 0)
	{
		SetColor(RED);
	}
	else
	{
		SetColor(BLUE);
	}
}


CParticle::~CParticle()
{
}

float CParticle::GetCharge()const
{
	return m_charge;
}

float CParticle::GetMass()const
{
	return m_mass;
}

glm::vec2 CParticle::GetPosition()const
{
	return m_pos;
}

glm::vec2 CParticle::GetVelocity()const
{
	return m_velocity;
}

glm::vec2 CParticle::GetAcceleration()const
{
	return m_acceleration;
}

void CParticle::SetPosition(glm::vec2 pos)
{
	m_pos = pos;
}

void CParticle::SetVelocity(glm::vec2 velocity)
{
	m_velocity = velocity;
}

void CParticle::SetAcceleration(glm::vec2 acceleration)
{
	m_acceleration = acceleration;
}

void CParticle::UpdateVelocity(glm::vec2 velocity)
{
	m_velocity += velocity;
}

void CParticle::UpdateAcceleration(glm::vec2 acceleration)
{
	m_acceleration += acceleration;
}

void CParticle::Update()
{
	m_velocity += m_acceleration;
	m_pos += m_velocity;
}

void CParticle::Redraw()const
{
	const glm::vec3 offset = { m_pos.x, m_pos.y, 0.f };
	const glm::mat4 transform = glm::translate(glm::mat4(), offset);
	
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(transform));

	DrawFilledCircle();

	if (m_charge > 0)
	{
		DrawPlus();
	}
	else
	{
		DrawMinus();
	}

	glPopMatrix();
}

void CParticle::DrawFilledCircle() const
{
	glColor3f(m_color.x, m_color.y, m_color.z);
	const int triangleAmount = 12; 
	const float radius = POINT_SIZE / 2;
	const float twicePi = 2.0f * (float)M_PI;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0);

	for (int i = 0; i <= triangleAmount; i++) 
	{
		float circleVertex = i * twicePi / triangleAmount;
		glVertex2f((radius * cos(circleVertex)), (radius * sin(circleVertex)));
	}

	glEnd();
}

void CParticle::DrawMinus()const
{
	glLineWidth(3);
	glColor3d(1, 1, 1);
	
	glBegin(GL_LINES);
	     
	glVertex2f(- POINT_SIZE / 4, 0);
	glVertex2f(POINT_SIZE / 4, 0);
	
	glEnd();
}

void CParticle::DrawPlus()const
{
	glLineWidth(3);
	glColor3d(1, 1, 1);

	glBegin(GL_LINES);

	glVertex2f(0, -POINT_SIZE / 4);
	glVertex2f(0, POINT_SIZE / 4);
	glVertex2f(-POINT_SIZE / 4, 0);
	glVertex2f(POINT_SIZE / 4, 0);

	glEnd();
}
