#include "stdafx.h"
#include "Arena.h"
#include <algorithm>

namespace
{
	const float TILE_SIZE = 15.f;

	glm::vec3 GetPosition(const Function2D &fn, float x, float z)
	{
		const float y = fn(x, z);
		return{ x, y, z };
	}

	void CalculateTriangleStripIndicies(std::vector<uint32_t> &indicies,
		unsigned columnCount, unsigned rowCount)
	{
		indicies.clear();
		indicies.reserve((columnCount - 1) * rowCount * 2);
		// вычисл€ем индексы вершин.
		for (unsigned ci = 0; ci < columnCount - 1; ++ci)
		{
			if (ci % 2 == 0)
			{
				for (unsigned ri = 0; ri < rowCount; ++ri)
				{
					unsigned index = ci * rowCount + ri;
					indicies.push_back(index + rowCount);
					indicies.push_back(index);
				}
			}
			else
			{
				for (unsigned ri = rowCount - 1; ri < rowCount; --ri)
				{
					unsigned index = ci * rowCount + ri;
					indicies.push_back(index);
					indicies.push_back(index + rowCount);
				}
			}
		}
	}
}

CArena::CArena(float xSize, float ySize)
{
	glm::vec4 green = { 0, 1, 0, 1 };
	m_material.SetAmbient(green);
	m_material.SetDiffuse(green);
	m_material.SetSpecular(green);
	m_material.SetShininess(30.f);

	Tesselate(xSize, ySize);
}

void CArena::Tesselate(float xSize, float ySize)
{
	m_mesh.Clear(MeshType::TriangleStrip);
	m_mesh.m_vertices.reserve(xSize * ySize);
	// вычисл€ем позиции вершин.
	for (float ci = -(xSize / 2); ci < (xSize / 2); ++ci)
	{
		const float x = TILE_SIZE * float(ci) / float(xSize - 1);
		for (float ri = -(ySize / 2); ri < (ySize / 2); ++ri)
		{
			const float y = TILE_SIZE * float(ri) / float(ySize - 1);

			SVertexP3NT2 vertex;
			vertex.position = { x, -0.1f, y };

			// Ќормаль к сфере - это нормализованный вектор радиуса к данной точке
			// ѕоскольку координаты центра равны 0, координаты вектора радиуса
			// будут равны координатам вершины.
			// Ѕлагодар€ радиусу, равному 1, нормализаци€ не требуетс€.
			vertex.normal = GetNormal(x, y);

			// ќбе текстурные координаты должны плавно измен€тьс€ от 0 до 1,
			// нат€гива€ пр€моугольную картинку на тело вращени€.
			// ѕри UV-параметризации текстурными координатами будут u и v.
			vertex.texCoord = { 1.f - x, y };

			m_mesh.m_vertices.push_back(vertex);
		}
	}

	CalculateTriangleStripIndicies(m_mesh.m_indicies, xSize, ySize);

}
glm::vec3 CArena::GetNormal(const float x, const float y)
{
	glm::vec3 v1 = { x, y, 0};
	glm::vec3 v2 = { x + 0.1f, y, 0 };
	glm::vec3 v3 = { x, y + 0.1f, 0 };
	return glm::normalize(glm::cross(v2 - v1, v3 - v1));
}

void CArena::Draw() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_material.Setup();
	m_mesh.Draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
