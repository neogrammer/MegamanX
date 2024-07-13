#include "SandboxState.hpp"
#include <stage/stages/SandboxStage.hpp>

SandboxState::SandboxState()
	: GameState{GameStateType::Sandbox}
	, m_stage{}
{
	m_stage = std::make_unique<SandboxStage>();
}

void SandboxState::Enter()
{
	if (m_stage)
	{
		m_stage.reset(nullptr);
		m_stage = std::make_unique<SandboxStage>();

	}
}

void SandboxState::Leave()
{
	if (m_stage)
	{
		m_stage.reset(nullptr);
	}
}

void SandboxState::ProcessInput()
{
	m_stage->ProcessInput();
}

void SandboxState::HandleEvent(const sf::Event& l_e)
{
	m_stage->HandleEvent(l_e);

}

void SandboxState::Update(const sf::Time& l_dt)
{
	m_stage->Update(l_dt);

}

void SandboxState::Render(sf::RenderWindow& l_wnd)
{
	m_stage->Render(l_wnd);

}
