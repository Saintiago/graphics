#pragma once
#include "ColoredShape.h"

static const float POINT_SIZE = 0.5;

class CParticle :
	public CColoredShape
{
public:

	CParticle(glm::vec2 pos, float charge);
	~CParticle();

	void Update();
	void SetPosition(glm::vec2 pos);
	void SetVelocity(glm::vec2 pos);
	void SetAcceleration(glm::vec2 pos);
	void UpdateVelocity(glm::vec2 pos);
	void UpdateAcceleration(glm::vec2 pos);

	float GetCharge()const;
	float GetMass()const;
	glm::vec2 GetPosition()const;
	glm::vec2 GetVelocity()const;
	glm::vec2 GetAcceleration()const;

protected:
	void Redraw()const override;

private:
	float m_charge = 1;
	float m_mass = 1000;
	glm::vec2 m_pos = { 0, 0 };
	glm::vec2 m_velocity = { 0, 0 };
	glm::vec2 m_acceleration = { 0, 0 };

	void DrawFilledCircle()const;
	void DrawMinus()const;
	void DrawPlus()const;
};

