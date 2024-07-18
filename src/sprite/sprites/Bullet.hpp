
#include <sprite/sprites/Projectile.hpp>
#ifndef BULLET_HPP__
#define BULLET_HPP__

struct Bullet : Projectile
{

	Bullet();
	Bullet(sf::Texture& l_tex, bool l_friendly = true);
	virtual ~Bullet() override = default;

	Bullet(const Bullet&);
	Bullet& operator=(const Bullet&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;
};

#endif