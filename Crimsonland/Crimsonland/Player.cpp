#include "stdafx.h"
#include "Player.h"

namespace
{
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

	const float MATERIAL_SHININESS = 30.f;
	
	const glm::vec4 BLACK_RGBA = { 0, 0, 0, 1 };
	const glm::vec4 FADED_WHITE_RGBA = { 0.3f, 0.3f, 0.3f, 1.f };
	const glm::vec4 YELLOW_RGBA = { 1, 1, 0, 1 };
	const glm::vec4 RED_RGBA = { 1, 0, 0, 1 };
	const glm::vec4 BLUE_RGBA = { 0, 0, 1, 1 };
	const glm::vec4 LIGHT_BLUE_RGBA = { 0.5f, 0.5f, 1, 1 };
	const glm::vec4 DARK_BLUE_RGBA = { 0, 0, 0.5f, 1 };
	const glm::vec4 GREEN_RGBA = { 0, 1, 0, 1 };
	const glm::vec4 WHITE_RGBA = { 1, 1, 1, 1 };

	const float LINEAR_MOVE_SPEED = 5.0f;

	bool ShouldTrackKeyPressed(const SDL_Keysym &key)
	{
		switch (key.sym)
		{
		case SDLK_w:
		case SDLK_a:
		case SDLK_s:
		case SDLK_d:
			return true;
		}
		return false;
	}

	glm::vec2 GetMoveDirection(std::set<unsigned> & keysPressed)
	{
		glm::vec2 direction = { 0, 0 };
		if (keysPressed.count(SDLK_UP) || keysPressed.count(SDLK_w))
		{
			direction.y -= 1;
		}
		if (keysPressed.count(SDLK_DOWN) || keysPressed.count(SDLK_s))
		{
			direction.y += 1;;
		}
		if (keysPressed.count(SDLK_LEFT) || keysPressed.count(SDLK_a))
		{
			direction.x -= 1;;
		}
		if (keysPressed.count(SDLK_RIGHT) || keysPressed.count(SDLK_d))
		{
			direction.x += 1;;
		}
		return direction;
	}
}

CPlayer::CPlayer()
{
	m_material.SetAmbient(RED_RGBA);
	m_material.SetDiffuse(YELLOW_RGBA);
	m_material.SetSpecular(FADED_WHITE_RGBA);
	m_material.SetShininess(MATERIAL_SHININESS);

	Tesselate();
}


CPlayer::~CPlayer()
{
}

glm::vec2 CPlayer::GetPosition()
{
	return m_pos;
}

void CPlayer::Update(float dt)
{
	SetTurretAngle();
	SetPosition(dt);
}

void CPlayer::Draw() const
{
	glPolygonMode(GL_FRONT, GL_FILL);
	m_material.Setup();

	const glm::vec3 eye = { 0, 0, 0 };
	const glm::vec3 center = { m_turretDirection.x, 0, m_turretDirection.y };
	const glm::vec3 up = { 0, -1, 0 };

	glm::mat4 transform = glm::translate(glm::mat4(), { m_pos.x, 0.f, m_pos.y });
	glm::mat4 directionMatrix = glm::lookAt(eye, center, up);
	transform = transform * directionMatrix;

	glPushMatrix();
	glMultMatrixf(glm::value_ptr(transform));

	m_mesh.Draw();

	glPopMatrix();
}

void CPlayer::Tesselate()
{
	m_mesh.Clear(MeshType::TriangleStrip);
	m_mesh.m_vertices.reserve(4);

	SVertexP3NT2 vertex1;
	SVertexP3NT2 vertex2;
	SVertexP3NT2 vertex3;
	SVertexP3NT2 vertex4;

	glm::vec3 v1 = { 0, 0, -2.0f };
	glm::vec3 v2 = { 1.0f, 0, 1.0f };
	glm::vec3 v3 = { -1.0f, 0, 1.0f };
	glm::vec3 v4 = { 0, -3.f, 1.0f };
	glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

	vertex1.position = v1;
	vertex2.position = v2;
	vertex3.position = v3;
	vertex4.position = v4;

	vertex1.normal = normal;
	vertex2.normal = normal;
	vertex3.normal = normal;
	vertex4.normal = normal;
	
	vertex1.texCoord = { 1.f - v1.x, v1.y };
	vertex2.texCoord = { 1.f - v2.x, v2.y };
	vertex3.texCoord = { 1.f - v3.x, v3.y };
	vertex4.texCoord = { 1.f - v4.x, v4.y };

	m_mesh.m_vertices.push_back(vertex1);
	m_mesh.m_vertices.push_back(vertex2);
	m_mesh.m_vertices.push_back(vertex3);
	m_mesh.m_vertices.push_back(vertex4);

	m_mesh.m_indicies.clear();
	m_mesh.m_indicies.reserve(3);

	m_mesh.m_indicies.push_back(1);
	m_mesh.m_indicies.push_back(2);
	m_mesh.m_indicies.push_back(0);

	m_mesh.m_indicies.push_back(3);
	m_mesh.m_indicies.push_back(0);
	m_mesh.m_indicies.push_back(2);

	m_mesh.m_indicies.push_back(3);
	m_mesh.m_indicies.push_back(1);
	m_mesh.m_indicies.push_back(0);

	m_mesh.m_indicies.push_back(3);
	m_mesh.m_indicies.push_back(2);
	m_mesh.m_indicies.push_back(1);
	
	//CalculateTriangleStripIndicies(m_mesh.m_indicies, 2, 2);
}

bool CPlayer::OnKeyDown(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.insert(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

bool CPlayer::OnKeyUp(const SDL_KeyboardEvent &event)
{
	if (ShouldTrackKeyPressed(event.keysym))
	{
		m_keysPressed.erase(unsigned(event.keysym.sym));
		return true;
	}
	return false;
}

bool CPlayer::OnMouseMotion(const CRay &ray)
{
	// Опираемся на соглашение, по которому
	//  все спрайты лежат в плоскости Oxz.
	CPlane plane({ 1, 0, 1 }, { 1, 0, 0 }, { 0, 0, 1 });
	SRayIntersection intersection;

	if (!plane.Hit(ray, intersection))
	{
		return false;
	}

	const glm::vec3 hitPoint3D = intersection.m_point;
	const glm::vec2 hitPoint(hitPoint3D.x, hitPoint3D.z);

	m_mousePos = hitPoint;
	
	return true;
}

void CPlayer::SetTurretAngle()
{
	glm::vec2 turretDirection = m_mousePos - m_pos;
	if (glm::length(turretDirection) > 0)
	{
		m_turretDirection = glm::normalize(turretDirection);
	}
}

void CPlayer::SetPosition(float dt)
{
	glm::vec2 direction = GetMoveDirection(m_keysPressed);
	if (glm::length(direction) > 0)
	{
		m_pos += glm::normalize(direction) * LINEAR_MOVE_SPEED * dt;
	}
}
