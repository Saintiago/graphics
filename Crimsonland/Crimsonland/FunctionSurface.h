#pragma once

#include "MeshP3NT2.h"
#include "CrimsonlandLib.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

using Function2D = std::function<float(float, float)>;

// Вершина с трёхмерной позицией и нормалью.
struct SVertexP3N
{
    glm::vec3 position;
    glm::vec3 normal;

    SVertexP3N() = default;
    SVertexP3N(const glm::vec3 &position)
        : position(position)
    {
    }
};

class CDottedFunctionSurface final : public ISceneObject
{
public:
    CDottedFunctionSurface(const Function2D &fn);

    /// Инициализирует сетку треугольников
    /// @param rangeX - диапазон, где x - нижняя граница, y - верхняя граница
    /// @param rangeZ - диапазон, где x - нижняя граница, y - верхняя граница
    void Tesselate(const glm::vec2 &rangeX, const glm::vec2 &rangeZ, float step);

    // IBody interface.
    void Update(float) final {}
    void Draw() const final;

private:
    Function2D m_fn;
    std::vector<SVertexP3N> m_vertices;
};

class CSolidFunctionSurface final : public ISceneObject
{
public:
    CSolidFunctionSurface(unsigned slices, unsigned stacks);

    /// Инициализирует индексированную сетку треугольников
    /// @param rangeX - диапазон, где x - нижняя граница, y - верхняя граница
    /// @param rangeZ - диапазон, где x - нижняя граница, y - верхняя граница
	void Tesselate(unsigned slices, unsigned stacks);

    // IBody interface.
    void Update(float) final {}
    void Draw() const final;

private:
    Function2D m_fn;
    std::vector<SVertexP3N> m_vertices;
    std::vector<uint32_t> m_indicies;

	SMeshP3NT2 m_mesh;

	glm::vec3 GetPositionOnHelicoid(float u, float v);
	void BuildNormals();
	glm::vec3 GetNormal(const float u, const float v);
};
