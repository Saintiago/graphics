#pragma once
#include "Particle.h"

static const glm::vec2 VIRTUAL_FIELD_SIZE = { 10, 10 };

class CParticleSystem
{
public:

	static const glm::vec2 center;

	CParticleSystem();
	~CParticleSystem();

	void RemoveAllParticles();
	void Populate(int particlesCount);
	void SpawnParticle(const glm::vec2 pos, const float charge);
	void Update(float deltaSeconds);
	void Draw();
	bool DeleteParticleByCoords(const glm::vec2);

private:
	
	void MakeBorderInteraction();
	bool IsCollisionOccured(const CParticle &particle1, const CParticle &particle2);
	glm::vec2 GetCoulombForce(const CParticle &particle1, const CParticle &particle2);
	glm::vec2 GetAcceleration(float mass, glm::vec2 coulomb);

	std::vector<CParticle> m_particles;
};

