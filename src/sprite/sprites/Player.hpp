#include <sprite/ASprite.hpp>
#include <action/ActionTarget.hpp>

#ifndef PLAYER_HPP__
#define PLAYER_HPP__

struct Player : ASprite, ActionTarget<int>
{
	Player();
	~Player() override = default;

	Player(const Player&);
	Player& operator=(const Player&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;

private:
	void bindActions();
};

#endif