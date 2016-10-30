#include "stdafx.h"
#include "FunctionSurface.h"
#include <algorithm>

namespace
{

const float DOT_SIZE = 5.f;
const unsigned MIN_PRECISION = 4;

glm::vec3 GetPosition(const Function2D &fn, float x, float z)
{
    const float y = fn(x, z);
    return {x, y, z};
}

// вычисляет нормали численным методом,
// с помощью векторного произведения.
void CalculateNormals(std::vector<SVertexP3N> &vertices,
                      const Function2D &fn, float step)
{
    for (SVertexP3N &v : vertices)
    {
        const glm::vec3 &position = v.position;
        glm::vec3 dir1 = GetPosition(fn, position.x, position.z + step) - position;
        glm::vec3 dir2 = GetPosition(fn, position.x + step, position.z) - position;
        v.normal = glm::normalize(glm::cross(dir1, dir2));
    }
}

/// Привязывает вершины к состоянию OpenGL,
/// затем вызывает 'callback'.
template <class T>
void DoWithBindedArrays(const std::vector<SVertexP3N> &vertices, T && callback)
{
    // Включаем режим vertex array и normal array.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Выполняем привязку vertex array и normal array
    const size_t stride = sizeof(SVertexP3N);
    glNormalPointer(GL_FLOAT, stride, glm::value_ptr(vertices[0].normal));
    glVertexPointer(3, GL_FLOAT, stride, glm::value_ptr(vertices[0].position));

    // Выполняем внешнюю функцию.
    callback();

    // Выключаем режим vertex array и normal array.
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void CalculateTriangleStripIndicies(std::vector<uint32_t> &indicies,
                                    unsigned columnCount, unsigned rowCount)
{
    indicies.clear();
    indicies.reserve((columnCount - 1) * rowCount * 2);
    // вычисляем индексы вершин.
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

CDottedFunctionSurface::CDottedFunctionSurface(const Function2D &fn)
    : m_fn(fn)
{
}

void CDottedFunctionSurface::Tesselate(const glm::vec2 &rangeX, const glm::vec2 &rangeZ, float step)
{
    m_vertices.clear();
    // вычисляем позиции вершин.
    for (float x = rangeX.x; x < rangeX.y; x += step)
    {
        for (float z = rangeZ.x; z < rangeZ.y; z += step)
        {
            m_vertices.push_back(SVertexP3N(GetPosition(m_fn, x, z)));
        }
    }
    CalculateNormals(m_vertices, m_fn, step);
}

void CDottedFunctionSurface::Draw() const
{
    glPointSize(DOT_SIZE);
    DoWithBindedArrays(m_vertices, [this] {
        glDrawArrays(GL_POINTS, 0, GLsizei(m_vertices.size()));
    });
}

CSolidFunctionSurface::CSolidFunctionSurface(unsigned slices, unsigned stacks)
{
	Tesselate(slices, stacks);
}

void CSolidFunctionSurface::Tesselate(unsigned slices, unsigned stacks)
{
	assert((slices >= MIN_PRECISION) && (stacks >= MIN_PRECISION));

	m_mesh.Clear(MeshType::TriangleStrip);
	m_mesh.m_vertices.reserve(slices * stacks);
	// вычисляем позиции вершин.
	for (unsigned ci = 0; ci < slices; ++ci)
	{
		const float u = 2 * float(ci) / float(slices - 1) - 1;
		for (unsigned ri = 0; ri < stacks; ++ri)
		{
			const float v = 2 * M_PI * float(ri) / float(stacks - 1) - M_PI;

			SVertexP3NT2 vertex;
			vertex.position = GetPositionOnHelicoid(u, v);

			// Нормаль к сфере - это нормализованный вектор радиуса к данной точке
			// Поскольку координаты центра равны 0, координаты вектора радиуса
			// будут равны координатам вершины.
			// Благодаря радиусу, равному 1, нормализация не требуется.
			vertex.normal = GetNormal(u, v);

			// Обе текстурные координаты должны плавно изменяться от 0 до 1,
			// натягивая прямоугольную картинку на тело вращения.
			// При UV-параметризации текстурными координатами будут u и v.
			vertex.texCoord = { 1.f - u, v };

			m_mesh.m_vertices.push_back(vertex);
		}
	}

	CalculateTriangleStripIndicies(m_mesh.m_indicies, slices, stacks);

	//BuildNormals();
}

glm::vec3 CSolidFunctionSurface::GetNormal(const float u, const float v)
{
	glm::vec3 v1 = GetPositionOnHelicoid(u, v);
	glm::vec3 v2 = GetPositionOnHelicoid(u + 0.1f, v + 0.1f);
	glm::vec3 v3 = GetPositionOnHelicoid(u - 0.1f, v - 0.1f);
	return glm::normalize(glm::cross(v2 - v1, v3 - v1));
}

void CSolidFunctionSurface::BuildNormals()
{
	for (uint32_t i = 0; i < m_mesh.m_indicies.size() - 2; i += 2)
	{
		glm::vec3 v1 = m_mesh.m_vertices.at(m_mesh.m_indicies.at(i)).position;
		glm::vec3 v2 = m_mesh.m_vertices.at(m_mesh.m_indicies.at(i + 1)).position;
		glm::vec3 v3 = m_mesh.m_vertices.at(m_mesh.m_indicies.at(i + 2)).position;
		glm::vec3 normal = -1.0f * glm::normalize(glm::cross(v2 - v1, v3 - v1));

		m_mesh.m_vertices.at(m_mesh.m_indicies.at(i)).normal = normal;
		m_mesh.m_vertices.at(m_mesh.m_indicies.at(i + 1)).normal = normal;
		m_mesh.m_vertices.at(m_mesh.m_indicies.at(i + 2)).normal = normal;
	}
}

/*void CSolidFunctionSurface::Tesselate(float uMax, float vMax, float h)
{
	m_mesh.Clear(MeshType::TriangleStrip);
	m_mesh.m_vertices.reserve((unsigned)(h * uMax * vMax));
	// вычисляем позиции вершин.
	
	for (float v = -(vMax / 2); v < (vMax / 2); v += 0.1f)
	{
		for (float u = -(uMax / 2); u < (uMax / 2); u += 0.1f)
		{
			SVertexP3NT2 vertex;
			vertex.position = GetPositionOnHelicoid(u, v, h);

			// Нормаль к сфере - это нормализованный вектор радиуса к данной точке
			// Поскольку координаты центра равны 0, координаты вектора радиуса
			// будут равны координатам вершины.
			// Благодаря радиусу, равному 1, нормализация не требуется.
			vertex.normal = vertex.position;

			// Обе текстурные координаты должны плавно изменяться от 0 до 1,
			// натягивая прямоугольную картинку на тело вращения.
			// При UV-параметризации текстурными координатами будут u и v.
			vertex.texCoord = { 1.f - u, v };

			m_mesh.m_vertices.push_back(vertex);
		}

		CalculateTriangleStripIndicies(m_mesh.m_indicies, 100, 100);
	}
	
}*/

glm::vec3 CSolidFunctionSurface::GetPositionOnHelicoid(float u, float v)
{
	float h = 0.5f;
	const float x = u * cos(v);
	const float y = u * sin(v);
	const float z = h * v;

	return{ x, z, y };
}

void CSolidFunctionSurface::Draw() const
{
	m_mesh.Draw();
}
