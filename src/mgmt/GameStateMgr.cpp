#include "GameStateMgr.hpp"
#include <game_state/game_states/SandboxState.hpp>

GameStateMgr::GameStateMgr()
	: m_stateMap{}
	, m_stateStack{}
{
	LoadMap();
}

void GameStateMgr::LoadMap()
{
	m_stateMap.emplace(std::pair{GameStateType::Sandbox, std::make_unique<SandboxState>()});
	m_stateStack.push(m_stateMap[GameStateType::Sandbox].get());
}

void GameStateMgr::PushStack(GameStateType l_type)
{
	m_stateStack.push(m_stateMap[l_type].get());
}

void GameStateMgr::PopStack()
{
	m_stateStack.pop();
}

GameState* GameStateMgr::Top()
{
	return m_stateStack.top();
}

void GameStateMgr::ProcessInput()
{
	Top()->ProcessInput();
}

void GameStateMgr::HandleEvent(const sf::Event& l_e)
{
	Top()->HandleEvent(l_e);

}

void GameStateMgr::Update(const sf::Time& l_dt)
{
	Top()->Update(l_dt);

}

void GameStateMgr::Render(sf::RenderWindow& l_wnd)
{
	Top()->Render(l_wnd);
}
