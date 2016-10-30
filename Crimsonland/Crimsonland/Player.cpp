#include "stdafx.h"
#include "Player.h"

namespace
{
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
	const float Z_DISTANCE = -20.0f;

	bool ShouldTrackKeyPressed(const SDL_Keysym &key)
	{
		switch (key.sym)
		{
		case SDLK_LEFT:
		case SDLK_RIGHT:
		case SDLK_UP:
		case SDLK_DOWN:
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
}


CPlayer::~CPlayer()
{
}

void CPlayer::Update(float dt)
{
	SetTurretAngle();
	SetPosition(dt);
}

void CPlayer::Draw()const
{
	m_material.Setup();

	const glm::vec3 eye = { 0, 0, 0 };
	const glm::vec3 center = { m_turretDirection.x, 0, m_turretDirection.y };
	const glm::vec3 up = { 0, -1, 0 };

	glm::mat4 transform = glm::translate(glm::mat4(), { m_pos.x, 0.f, m_pos.y });
	glm::mat4 directionMatrix = glm::lookAt(eye, center, up);
	transform = transform * directionMatrix;

	glm::vec3 v1 = { 0, 0, -0.5f };
	glm::vec3 v2 = { 0.25f, 0, 0.25f };
	glm::vec3 v3 = { -0.25f, 0, 0.25f };
	glm::vec3 normal = -1.0f * glm::normalize(glm::cross(v2 - v1, v3 - v1));

	glPushMatrix();
	glMultMatrixf(glm::value_ptr(transform));

	glBegin(GL_TRIANGLES);

	glNormal3f(normal.x, normal.y, normal.z);
	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v3.x, v3.y, v3.z);
	glVertex3f(v2.x, v2.y, v2.z);

	glEnd();

	glPopMatrix();
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
