#pragma once

#include <glm/vec3.hpp>
#include <boost/noncopyable.hpp>

class CGrid : private boost::noncopyable
{
public:
	CGrid();
	~CGrid();

	void SetWindowSize(const glm::vec2 &size);
	void SetupColor(const glm::vec3 &gridColor);
	void SetXAxisSpan(const glm::vec2 &gridXSpan);
	void SetYAxisSpan(const glm::vec2 &gridYSpan);
	void Draw()const;

private:
	void DeleteList();
	void Redraw()const;
	mutable unsigned m_displayList = 0;

	glm::vec2 m_spanX;
	glm::vec2 m_spanY;
	glm::vec2 m_windowSize;
	glm::vec3 m_gridColor;
};

