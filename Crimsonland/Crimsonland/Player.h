#pragma once
#include "CrimsonlandLib.h"
#include "Lights.h"
#include <set>

class CPlayer :
	public ISceneObject
{
public:
	CPlayer();
	~CPlayer();

	void Update(float dt);
	void Draw()const;

	bool OnKeyDown(const SDL_KeyboardEvent &event);
	bool OnKeyUp(const SDL_KeyboardEvent &event);
	bool OnMouseMotion(const CRay &ray);

private:

	CPhongModelMaterial m_material;
	std::set<unsigned> m_keysPressed;
	glm::vec2 m_pos = { 0, 0 };
	glm::vec2 m_mousePos = { 0, 0 };
	glm::vec2 m_turretDirection = { 0, 1 };

	void SetTurretAngle();
	void SetPosition(float dt);
};

