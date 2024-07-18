#include "Projectile.hpp"
#include <res/Cfg.hpp>

Projectile::Projectile()
	: ASprite{}
{

}

Projectile::Projectile(sf::Texture& l_tex, SpriteName l_name, bool l_friendly)
	: ASprite{ SpriteType::Projectile, SpriteName::Count, l_tex }
	, m_friendly{ l_friendly }
	, m_damage{ 1 }
{
	
}


Projectile::Projectile(const Projectile& other)
	: ASprite(other)
	, m_friendly{ other.m_friendly }
	, m_damage{ other.m_damage }
{
	spr_ = other.spr_;
	spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
}

Projectile& Projectile::operator=(const Projectile& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	m_friendly = other.m_friendly;
	m_damage = other.m_damage;
	return *this;
}


bool Projectile::GetFriendly()
{
	return m_friendly;
}

int Projectile::GetDamage()
{
	return m_damage;
}
