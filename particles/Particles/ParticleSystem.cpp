#include "stdafx.h"
#include "ParticleSystem.h"

namespace
{
	const float VIRTUAL_POINT_BORDER = POINT_SIZE * 1;
}

CParticleSystem::CParticleSystem()
{
}


CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::RemoveAllParticles()
{
	m_particles.clear();
}

bool CParticleSystem::DeleteParticleByCoords(const glm::vec2 coords)
{
	for (unsigned i = 0; i < m_particles.size(); ++i)
	{
		float distance = glm::distance(coords, m_particles[i].GetPosition());
		if (distance <= POINT_SIZE / 2)
		{
			m_particles.erase(m_particles.begin() + i);
			return true;
		}
	}
	return false;
}

void CParticleSystem::SpawnParticle(const glm::vec2 pos = { 0,0 }, const float charge = 1)
{
	CParticle pParticle(pos, charge);
	pParticle.SetVelocity({0.001, 0.001});
	m_particles.push_back(pParticle);
}

void CParticleSystem::Update(float deltaSeconds)
{
	// Skip too long time deltas (like after game pause).
	if (deltaSeconds > 0.1)
	{
		deltaSeconds = 0.1f;
	}

	MakeBorderInteraction();
	for (auto &particle : m_particles)
	{
		glm::vec2 acceleration = { 0, 0 };
		for (const auto &anotherParticle : m_particles)
		{
			if (&particle == &anotherParticle)
			{
				continue;
			}
			acceleration += GetAcceleration(particle.GetMass(), GetCoulombForce(particle, anotherParticle));
		}
		particle.SetAcceleration(acceleration * deltaSeconds);
	}

	for (auto &particle : m_particles)
	{
		particle.Update();
	}
}



void CParticleSystem::Draw()
{
	for (const auto &pParticle : m_particles)
	{
		pParticle.Draw();
	}
}

void CParticleSystem::MakeBorderInteraction()
{
	for (auto &particle : m_particles)
	{
		const glm::vec2 pos = particle.GetPosition();
		const float radius = POINT_SIZE / 2;
		glm::vec2 newPos = pos;
		glm::vec2 deltaVec = { 1, 1 };
		
		if (pos.x < radius)
		{
			deltaVec.x = -1;
			newPos.x = radius;
		}
		if (pos.x > VIRTUAL_FIELD_SIZE.x - radius)
		{
			deltaVec.x = -1;
			newPos.x = VIRTUAL_FIELD_SIZE.x - radius;
		}
		if (pos.y < radius)
		{
			deltaVec.y = -1;
			newPos.y = radius;
		}
		if (pos.y > VIRTUAL_FIELD_SIZE.y - radius)
		{
			deltaVec.y = -1;
			newPos.y = VIRTUAL_FIELD_SIZE.y - radius;
		}

		particle.SetPosition(newPos);
		particle.SetVelocity(particle.GetVelocity() * deltaVec);
	}
}

bool CParticleSystem::IsCollisionOccured(const CParticle &particle1, const CParticle &particle2)
{
	return glm::distance(particle1.GetPosition(), particle2.GetPosition()) <= VIRTUAL_POINT_BORDER / 2;
}

glm::vec2 CParticleSystem::GetCoulombForce(const CParticle &particle1, const CParticle &particle2)
{
	float resCharge = particle1.GetCharge() * particle2.GetCharge();
	const glm::vec2 radiusVec = particle1.GetPosition() - particle2.GetPosition();
	const float distance = glm::length(radiusVec);

	if (resCharge == -1 && distance < VIRTUAL_POINT_BORDER)
	{
		resCharge *= -2;
	}

	if (distance > 0.00001)
	{
		return (resCharge * radiusVec) / std::powf(distance, 3);
	}
	else
	{
		return { 0,0 };
	}
}

glm::vec2 CParticleSystem::GetAcceleration(float mass, glm::vec2 force)
{
	return force / mass;
}

void CParticleSystem::Populate(int particlesCount)
{
	float charge = 1;

	for (int i = 0; i < particlesCount; i++)
	{
		charge *= -1;
		float coordX = (float)(rand() % 1000) / 100;
		float coordY = (float)(rand() % 1000) / 100;
		SpawnParticle({ coordX, coordY }, charge);
	}
}