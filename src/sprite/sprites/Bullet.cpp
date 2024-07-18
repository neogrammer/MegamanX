#include "Bullet.hpp"
#include <res/Cfg.hpp>

Bullet::Bullet()
	: Projectile{}
{

}

Bullet::Bullet(sf::Texture& l_tex, bool l_friendly)
	: Projectile{ l_tex, SpriteName::BusterBullet, l_friendly }
{
	animMgr.AddAnimation(spr_, l_tex, AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
}


Bullet::Bullet(const Bullet& other)
	: Projectile(other)
{
	animMgr = AnimationMgr{};
	animMgr.AddAnimation(spr_, *const_cast<sf::Texture*>(spr_.getTexture()), AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
}

Bullet& Bullet::operator=(const Bullet& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	m_friendly = other.m_friendly;
	m_damage = other.m_damage;
	animMgr = AnimationMgr{};
	animMgr.AddAnimation(spr_, *const_cast<sf::Texture*>(spr_.getTexture()), AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
	
	return *this;
}

void Bullet::processInput()
{

}


void Bullet::update(const sf::Time& l_dt)
{
	gameTime_ = l_dt;
	animMgr.Update(l_dt);
}