#pragma once
#include "ColoredShape.h"

enum sign
{
	POSITIVE = 1,
	NEGATIVE = -1
};

class CParticle :
	public CColoredShape
{
public:

	static const float pointSize;

	CParticle(glm::vec2 pos, sign sign);
	~CParticle();

	sign GetSign();
	glm::vec2 GetPosition();

protected:
	void Redraw()const override;

private:
	sign m_sign;
	glm::vec2 m_pos;
};

