#ifndef GAMESTATE_HPP__
#define GAMESTATE_HPP__
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <misc/globals.hpp>
struct GameState
{
	GameState() = delete;
	explicit GameState(GameStateType l_type);
	virtual ~GameState() = default;

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;
	GameState(GameState&&) = delete;
	GameState& operator=(GameState&&) = delete;

	virtual void Enter() = 0;
	virtual void Leave() = 0;

	virtual void ProcessInput() = 0;
	virtual void HandleEvent(const sf::Event& l_e) = 0;
	virtual void Update(const sf::Time& l_dt) = 0;
	virtual void Render(sf::RenderWindow& l_wnd) = 0;

	GameStateType getType();

protected:

	GameStateType m_type;
};

#endif