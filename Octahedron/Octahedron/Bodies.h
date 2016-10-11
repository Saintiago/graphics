#pragma once

#include <glm/vec3.hpp>
#include <glm/fwd.hpp>
#include <boost/noncopyable.hpp>

enum class OctaFace
{
	FrontLeft = 0,
	FrontTop,
	FrontRight,
	FrontBottom,
	BackLeft,
	BackTop,
	BackRight,
	BackBottom,

	NumFaces
};

struct STriangleFace
{
	uint16_t vertexIndex1;
	uint16_t vertexIndex2;
	uint16_t vertexIndex3;
	uint16_t colorIndex;
};

class COctahedron
{
public:
	COctahedron();
	void Update(float deltaTime);
	void Draw()const;

	void SetFaceColor(OctaFace face, const glm::vec3 &color);

private:
	static const size_t COLORS_COUNT = static_cast<size_t>(OctaFace::NumFaces);
	glm::vec3 m_colors[COLORS_COUNT];

	void DrawMesh()const;
	void DrawTriangle(const STriangleFace &face)const;
	void DrawFaces()const;
};