#pragma once
#include "DispatchEvent.h"
#include <vector>
#include "ParticleSystem.h"

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
	void OnMouseUp(const SDL_MouseButtonEvent &) override;
	void OnKeyUp(const SDL_KeyboardEvent &) override;

private:
	void SetupView(const glm::ivec2 &size);
	glm::vec2 MapMousePosition(glm::vec2)const;

	std::unique_ptr<CParticleSystem> m_particles;
};
