#ifndef PROJECTILE_HPP__
#define PROJECTILE_HPP__

#include <sprite/ASprite.hpp>

#ifndef Projectile_HPP__
#define Projectile_HPP__

struct Projectile : ASprite
{

	Projectile();
	Projectile(sf::Texture& l_tex, SpriteName l_name, bool l_friendly = true, sf::IntRect l_rect = { {0,0},{0,0} }, sf::IntRect l_bbox = { {0,0},{0,0} });
	virtual ~Projectile() override = default;

	Projectile(const Projectile&);
	Projectile& operator=(const Projectile&);

	virtual void processInput() override = 0;
	virtual void update(const sf::Time& l_dt) override = 0;
	bool GetFriendly();
	int GetDamage();
protected:
	static int BULLETSPEED;
	bool m_friendly{};
	int m_damage{};
};

#endif

#endif