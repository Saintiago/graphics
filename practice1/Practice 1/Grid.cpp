#include "stdafx.h"
#include "Grid.h"
#include <cmath>

CGrid::CGrid()
{
}


CGrid::~CGrid()
{
	DeleteList();
}

void CGrid::SetWindowSize(const glm::vec2 &size)
{
	m_windowSize = size;
}

void CGrid::SetupColor(const glm::vec3 &gridColor)
{
	m_gridColor = gridColor;
}

void CGrid::SetXAxisSpan(const glm::vec2 &gridXSpan)
{
	m_spanX = gridXSpan;
}

void CGrid::SetYAxisSpan(const glm::vec2 &gridYSpan)
{
	m_spanY = gridYSpan;
}

void CGrid::Draw()const
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

void CGrid::Redraw()const
{
	glBegin(GL_LINES);

	glColor3f(m_gridColor.x, m_gridColor.y, m_gridColor.z);
	
	glVertex2f(0, m_windowSize.y / 2);
	glVertex2f(m_windowSize.x, m_windowSize.y / 2);

	glVertex2f(m_windowSize.x / 2, 0);
	glVertex2f(m_windowSize.x / 2, m_windowSize.y);

	glEnd();
}

void CGrid::DeleteList()
{
	if (m_displayList)
	{
		glDeleteLists(m_displayList, 1);
		m_displayList = 0;
	}
}