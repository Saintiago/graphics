#pragma once
#include "CrimsonlandLib.h"
#include <vector>
#include "Player.h"
#include "Arena.h"

class CWindowClient : public CAbstractWindowClient
{
public:
    CWindowClient(CWindow &window);

protected:
    // IWindowClient interface
    void OnUpdateWindow(float deltaSeconds) override;
    void OnDrawWindow() override;
    void OnKeyDown(const SDL_KeyboardEvent &) override;
    void OnKeyUp(const SDL_KeyboardEvent &) override;
	void OnMouseMotion(const glm::vec2 &pos) override;

private:
    void CheckOpenGLVersion();
    void SetupView(const glm::ivec2 &size);

    CCamera m_camera;
    CDirectedLightSource m_sunlight;
    CShaderProgram m_programPhong;
    CShaderProgram m_programLambert;
    CShaderProgram m_programFixed;
    std::vector<CShaderProgram *> m_programQueue;

	std::unique_ptr<CPlayer> m_player;
	std::unique_ptr<CArena> m_arena;
};
