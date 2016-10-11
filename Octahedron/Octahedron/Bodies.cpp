#include "stdafx.h"
#include "Bodies.h"
#include <stdint.h>

namespace
{

	const glm::vec3 BLACK = { 0.f, 0.f, 0.f };

typedef glm::vec3 Vertex;

const Vertex CUBE_VERTICIES[] = {
	{ -1, +1, -1 },
	{ +1, +1, -1 },
	{ +1, -1, -1 },
	{ -1, -1, -1 },
	{ -1, +1, +1 },
	{ +1, +1, +1 },
	{ +1, -1, +1 },
	{ -1, -1, +1 },
};


const Vertex OCTA_VERTICIES[] = {
	(CUBE_VERTICIES[0] + CUBE_VERTICIES[1] + CUBE_VERTICIES[4] + CUBE_VERTICIES[5]) / 4.f,
	(CUBE_VERTICIES[1] + CUBE_VERTICIES[2] + CUBE_VERTICIES[5] + CUBE_VERTICIES[6]) / 4.f,
	(CUBE_VERTICIES[2] + CUBE_VERTICIES[3] + CUBE_VERTICIES[6] + CUBE_VERTICIES[7]) / 4.f,
	(CUBE_VERTICIES[0] + CUBE_VERTICIES[3] + CUBE_VERTICIES[4] + CUBE_VERTICIES[7]) / 4.f,
	(CUBE_VERTICIES[4] + CUBE_VERTICIES[5] + CUBE_VERTICIES[6] + CUBE_VERTICIES[7]) / 4.f,
	(CUBE_VERTICIES[0] + CUBE_VERTICIES[1] + CUBE_VERTICIES[2] + CUBE_VERTICIES[3]) / 4.f
};

Vertex GetOctaVertex(uint16_t vertexIndex)
{
	return Vertex();
}

const STriangleFace OCTA_FACES[] = {
	{ 0, 3, 4, static_cast<uint16_t>(OctaFace::FrontLeft) },
	{ 0, 4, 1, static_cast<uint16_t>(OctaFace::FrontTop) },
	{ 1, 4, 2, static_cast<uint16_t>(OctaFace::FrontRight) },
	{ 3, 2, 4, static_cast<uint16_t>(OctaFace::FrontBottom) },
	{ 0, 5, 3, static_cast<uint16_t>(OctaFace::BackLeft) },
	{ 0, 1, 5, static_cast<uint16_t>(OctaFace::BackTop) },
	{ 1, 2, 5, static_cast<uint16_t>(OctaFace::BackRight) },
	{ 2, 3, 5, static_cast<uint16_t>(OctaFace::BackBottom) }
};

}

COctahedron::COctahedron()
{
	// Используем белый цвет по умолчанию.
	for (glm::vec3 &color : m_colors)
	{
		color.x = 1;
		color.y = 1;
		color.z = 1;
	}
}

void COctahedron::Update(float deltaTime)
{
	(void)deltaTime;
}

void COctahedron::Draw() const
{
	DrawMesh();
	DrawFaces();
}

void COctahedron::DrawMesh()const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5.0f);
	glColor3fv(glm::value_ptr(BLACK));

	glBegin(GL_TRIANGLES);

	for (const STriangleFace &face : OCTA_FACES)
	{
		DrawTriangle(face);
	}

	glEnd();
}

void COctahedron::DrawFaces()const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);

	for (const STriangleFace &face : OCTA_FACES)
	{
		glColor3fv(glm::value_ptr(m_colors[face.colorIndex]));
		DrawTriangle(face);
	}

	glEnd();
}

void COctahedron::DrawTriangle(const STriangleFace &face)const
{
	const Vertex &v1 = OCTA_VERTICIES[face.vertexIndex1];
	const Vertex &v2 = OCTA_VERTICIES[face.vertexIndex2];
	const Vertex &v3 = OCTA_VERTICIES[face.vertexIndex3];
	glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

	glNormal3fv(glm::value_ptr(normal));
	glVertex3fv(glm::value_ptr(v1));
	glVertex3fv(glm::value_ptr(v2));
	glVertex3fv(glm::value_ptr(v3));
}

void COctahedron::SetFaceColor(OctaFace face, const glm::vec3 &color)
{
	const size_t index = static_cast<size_t>(face);
	assert(index < COLORS_COUNT);
	m_colors[index] = color;
}