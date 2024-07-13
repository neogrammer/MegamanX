#include "GameState.hpp"


GameState::GameState(GameStateType l_type)
	: m_type{ l_type }
{}

GameStateType GameState::getType()
{
	return m_type;
}
