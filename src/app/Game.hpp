#ifndef GAME_HPP__
#define GAME_HPP__
#include <SFML/Graphics/RenderWindow.hpp>
#include "Wndw.hpp"
#include <mgmt/GameStateMgr.hpp>

struct Game
{
	Game() = default;
	~Game() = default;

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	void Run();

private:
	Wndw m_wnd = {};
	GameStateMgr m_stateMgr = {};

	void update(sf::Time l_dt);

	void render();

	void input();
};

#endif