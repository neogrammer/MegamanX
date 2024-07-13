#ifndef GAMESTATEMGR_HPP__
#define GAMESTATEMGR_HPP__

#include <unordered_map>
#include <stack>
#include <memory>
#include <misc/globals.hpp>

#include <game_state/GameState.hpp>

struct GameStateMgr
{
	GameStateMgr();
	~GameStateMgr() = default;

	GameStateMgr(const GameStateMgr&) = delete;
	GameStateMgr& operator=(const GameStateMgr&) = delete;
	GameStateMgr(GameStateMgr&&) = delete;
	GameStateMgr& operator=(GameStateMgr&&) = delete;

	void LoadMap();
	void PushStack(GameStateType l_type);
	void PopStack();
	GameState* Top();

	void ProcessInput();
	void HandleEvent(const sf::Event& l_e);
	void Update(const sf::Time& l_dt);
	void Render(sf::RenderWindow& l_wnd);

private:
	std::unordered_map<GameStateType, std::unique_ptr<GameState>> m_stateMap;
	std::stack<GameState*> m_stateStack;
};

#endif