#ifndef BUZZBIRD_HPP__
#define BUZZBIRD_HPP__

#include <sprite/sprites/Enemy.hpp>

struct BuzzBird : Enemy 
{
	BuzzBird();
	virtual ~BuzzBird() override = default;

	BuzzBird(const BuzzBird&);
	BuzzBird& operator=(const BuzzBird&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;

};

#endif