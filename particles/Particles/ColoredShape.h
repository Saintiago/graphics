#pragma once

#include <glm/vec3.hpp>
#include <boost/noncopyable.hpp>

static const glm::vec3 RED = { 1, 0, 0 };
static const glm::vec3 BLUE = { 0, 0, 1 };

class CColoredShape
{
public:
	CColoredShape() = default;
	~CColoredShape();

	void SetColor(const glm::vec3 &color);
	void Draw()const;

protected:
	glm::vec3 m_color;

	virtual void Redraw()const = 0;

};

