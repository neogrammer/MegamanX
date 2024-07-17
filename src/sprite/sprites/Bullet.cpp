#include "Bullet.hpp"
#include <res/Cfg.hpp>

Bullet::Bullet()
	: ASprite{}
{

}

Bullet::Bullet(sf::Texture& l_tex, bool l_friendly)
	: ASprite{SpriteType::Projectile, SpriteName::BusterBullet, l_tex}
	, m_friendly{l_friendly}
	, m_damage{ 1 }
{
	animMgr.AddAnimation(spr_, l_tex, AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
}


Bullet::Bullet(const Bullet& other)
	: ASprite(other)
	, m_friendly{ other.m_friendly }
	, m_damage{ other.m_damage }
{
	spr_ = other.spr_;
	spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	animMgr = AnimationMgr{};
	animMgr.AddAnimation(spr_, *const_cast<sf::Texture*>(spr_.getTexture()), AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
}

Bullet& Bullet::operator=(const Bullet& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	animMgr = AnimationMgr{};
	animMgr.AddAnimation(spr_, *const_cast<sf::Texture*>(spr_.getTexture()), AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
	m_friendly = other.m_friendly;
	m_damage = other.m_damage;
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

bool Bullet::GetFriendly()
{
	return m_friendly;
}

int Bullet::GetDamage()
{
	return m_damage;
}

