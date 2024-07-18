#ifndef PROJECTILE_HPP__
#define PROJECTILE_HPP__

#include <sprite/ASprite.hpp>

#ifndef Projectile_HPP__
#define Projectile_HPP__

struct Projectile : ASprite
{

	Projectile();
	Projectile(sf::Texture& l_tex, SpriteName l_name, bool l_friendly = true);
	virtual ~Projectile() override = default;

	Projectile(const Projectile&);
	Projectile& operator=(const Projectile&);

	virtual void processInput() override = 0;
	virtual void update(const sf::Time& l_dt) override = 0;
	bool GetFriendly();
	int GetDamage();
protected:
	bool m_friendly{};
	int m_damage{};
};

#endif

#endif