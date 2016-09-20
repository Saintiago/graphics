#pragma once

#include <glm/vec3.hpp>
#include <boost/noncopyable.hpp>

static const glm::vec3 RED = { 1, 0, 0 };;
static const glm::vec3 BLUE = { 0, 0, 1 };;

class CColoredShape : private boost::noncopyable
{
public:
	CColoredShape() = default;
	~CColoredShape();

	void SetColor(const glm::vec3 &color);
	void Draw()const;

protected:
	mutable unsigned m_displayList = 0;
	glm::vec3 m_color;

	void DeleteList();
	virtual void Redraw()const = 0;

};

