#pragma once
#include "src/ISceneObject.h"
#include "MeshP3NT2.h"
#include "CrimsonlandLib.h"
#include <functional>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

using Function2D = std::function<float(float, float)>;

// ������� � ��������� �������� � ��������.
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

class CArena final : public ISceneObject
{
public:
	CArena(unsigned slices, unsigned stacks);

	/// �������������� ��������������� ����� �������������
	/// @param rangeX - ��������, ��� x - ������ �������, y - ������� �������
	/// @param rangeZ - ��������, ��� x - ������ �������, y - ������� �������
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
	glm::vec3 GetNormal(const float u, const float v);
};
