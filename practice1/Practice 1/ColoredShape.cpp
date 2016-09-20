#include "stdafx.h"
#include "ColoredShape.h"

CColoredShape::~CColoredShape()
{
	DeleteList();
}

void CColoredShape::SetColor(const glm::vec3 &color)
{
	m_color = color;
}

void CColoredShape::SetWindowSize(const glm::vec2 &size)
{
	m_windowSize = size;
	m_windowCenter = { size.x / 2, size.y / 2 };
	m_step = { size.x / AXIS_X_STEPS, size.y / AXIS_Y_STEPS };
}

void CColoredShape::DeleteList()
{
	if (m_displayList)
	{
		glDeleteLists(m_displayList, 1);
		m_displayList = 0;
	}
}

void CColoredShape::Draw()const
{
	/*
	if (!m_displayList)
	{
	m_displayList = glGenLists(1);
	glNewList(m_displayList, GL_COMPILE);
	Redraw();
	glEndList();
	}

	glCallList(m_displayList);
	*/
	Redraw();
}