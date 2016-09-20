#pragma once
#include "ColoredShape.h"

class CParabola : public CColoredShape
{
public:
	CParabola();
	~CParabola();

	void SetXAxisSpan(const glm::vec2 &xSpan);

protected:
	void Redraw()const override;

private:
	glm::vec2 GetPosition(float x)const;

	glm::vec2 m_spanX;
};

