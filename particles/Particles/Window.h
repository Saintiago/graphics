#pragma once
#include "DispatchEvent.h"
#include <vector>
#include "Particle.h"

class CWindow : public CAbstractInputControlWindow
{
public:
	CWindow();

protected:
	// CAbstractWindow interface
	void OnUpdateWindow(float deltaSeconds) override;
	void OnDrawWindow(const glm::ivec2 &size) override;

	// IInputEventAcceptor interface
	void OnDragBegin(const glm::vec2 &pos) override;
	void OnDragMotion(const glm::vec2 &pos) override;
	void OnDragEnd(const glm::vec2 &pos) override;

private:
	void SetupView(const glm::ivec2 &size);

	glm::vec2 m_dragOffset;
	std::vector<std::unique_ptr<CParticle>> m_particles;
};
