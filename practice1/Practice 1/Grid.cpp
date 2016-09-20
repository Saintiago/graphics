#include "stdafx.h"
#include "Grid.h"
#include <cmath>

namespace
{
	const float STEP_STROKE_LENGTH = 10;
}

void CGrid::Redraw()const
{
	glBegin(GL_LINES);

	glColor3f(m_color.x, m_color.y, m_color.z);
	
	glm::vec2 middle = { m_windowSize.x / 2, m_windowSize.y / 2 };

	// X axis
	glVertex2f(0, middle.y);
	glVertex2f(m_windowSize.x, middle.y);

	// Y axis
	glVertex2f(middle.x, 0);
	glVertex2f(middle.x, m_windowSize.y);

	// X axis steps
	const float stepSizeX = m_windowSize.x / AXIS_X_STEPS;
	const float strokeTop = middle.y - (STEP_STROKE_LENGTH / 2);
	const float strokeBottom = strokeTop + STEP_STROKE_LENGTH;

	for (int i = 0; i < AXIS_X_STEPS; i++)
	{
		glVertex2f(stepSizeX * i, strokeTop);
		glVertex2f(stepSizeX * i, strokeBottom);
	}

	// Y axis steps
	const float stepSizeY = m_windowSize.y / AXIS_Y_STEPS;
	const float strokeLeft = middle.x - (STEP_STROKE_LENGTH / 2);
	const float strokeRight = strokeLeft + STEP_STROKE_LENGTH;

	for (int i = 0; i < AXIS_Y_STEPS; i++)
	{
		glVertex2f(strokeLeft, stepSizeY * i);
		glVertex2f(strokeRight, stepSizeY * i);
	}

	glEnd();

	glBegin(GL_TRIANGLES);

	// X arrow
	const float arrowLeft = m_windowSize.x - STEP_STROKE_LENGTH;
	glVertex2f(m_windowSize.x, middle.y);
	glVertex2f(arrowLeft, strokeTop);
	glVertex2f(arrowLeft, strokeBottom);

	// Y arrow
	const float arrowBottom = STEP_STROKE_LENGTH;
	glVertex2f(middle.x, 0);
	glVertex2f(strokeLeft, arrowBottom);
	glVertex2f(strokeRight, arrowBottom);

	glEnd();
}