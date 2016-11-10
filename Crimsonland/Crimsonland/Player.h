#pragma once
#include "CrimsonlandLib.h"
#include <set>
#include "MeshP3NT2.h"

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

	glm::vec2 GetPosition();

private:

	CPhongModelMaterial m_material;
	std::set<unsigned> m_keysPressed;
	glm::vec2 m_pos = { 0, 0 };
	glm::vec2 m_mousePos = { 0, 0 };
	glm::vec2 m_turretDirection = { 0, 1 };
	SMeshP3NT2 m_mesh;

	void SetTurretAngle();
	void SetPosition(float dt);
	void Tesselate();
};

