#pragma once
#include "CrimsonlandLib.h"
#include "Lights.h"
#include "FunctionSurface.h"
#include "Player.h"
#include "Arena.h"
#include <vector>

class CWindow : public CAbstractInputControlWindow
{
public:
    CWindow();

protected:
    // CAbstractWindow interface
    void OnWindowInit(const glm::ivec2 &size) override;
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow(const glm::ivec2 &size) override;

    // IInputEventAcceptor interface
    void OnKeyDown(const SDL_KeyboardEvent &) override;
    void OnKeyUp(const SDL_KeyboardEvent &) override;
	void OnMouseMotion(const glm::vec2 &pos) override;

private:
    void SetupView(const glm::ivec2 &size);
	glm::mat4 GetProjectionMatrix(const glm::ivec2 &size);

    CPhongModelMaterial m_material;
    CCamera m_camera;
    CDirectedLightSource m_sunlight;
	
	std::unique_ptr<CPlayer> m_player;
	std::unique_ptr<CArena> m_arena;
};
