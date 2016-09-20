#pragma once

#include <glm/vec3.hpp>
#include <boost/noncopyable.hpp>

namespace
{
	const int AXIS_X_STEPS = 20;
	const int AXIS_Y_STEPS = 20;
}

class CColoredShape : private boost::noncopyable
{
public:
	CColoredShape() = default;
	~CColoredShape();

	void SetWindowSize(const glm::vec2 &size);
	void SetColor(const glm::vec3 &color);
	void Draw()const;

protected:
	mutable unsigned m_displayList = 0;
	glm::vec3 m_color;
	glm::vec2 m_windowSize;
	glm::vec2 m_step;
	glm::vec2 m_windowCenter;

	void DeleteList();
	virtual void Redraw()const = 0;

};

