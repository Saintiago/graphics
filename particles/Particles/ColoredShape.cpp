#include "stdafx.h"
#include "ColoredShape.h"

CColoredShape::~CColoredShape()
{
}

void CColoredShape::SetColor(const glm::vec3 &color)
{
	m_color = color;
}

void CColoredShape::Draw()const
{
	Redraw();
}