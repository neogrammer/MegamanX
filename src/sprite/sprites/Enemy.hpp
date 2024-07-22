#ifndef ENEMY_HPP__
#define ENEMY_HPP__

#include <sprite/ASprite.hpp>

struct Enemy : ASprite
{
	Enemy(SpriteName l_name, sf::Texture& l_tex, sf::IntRect l_rect = {{0,0},{0,0}}, sf::IntRect l_bbox = { {0,0},{0,0} });
	virtual ~Enemy() override = default;

	Enemy(const Enemy&);
	Enemy& operator=(const Enemy&);

	void processInput() override = 0;
	void update(const sf::Time& l_dt) override = 0;
	bool IsShooting();
	void TakeHit(int l_damage);
	void Shoot();
	
protected:
	int m_hp{ 13 };
	bool m_isShooting{ false };
	sf::Clock m_shootTimer{};
};

#endif