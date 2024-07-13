#include <sprite/ASprite.hpp>

#ifndef PLAYER_HPP__
#define PLAYER_HPP__

struct Player : ASprite
{
	Player();
	~Player() override = default;

	Player(const Player&);
	Player& operator=(const Player&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;

private:

};

#endif