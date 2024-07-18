#include "Enemy.hpp"
#include <res/Cfg.hpp>
#include <iostream>


Enemy::Enemy(SpriteName l_name, sf::Texture& l_tex) : ASprite{ SpriteType::Enemy, l_name, l_tex }
{}

Enemy::Enemy(const Enemy&)
{
}

Enemy& Enemy::operator=(const Enemy& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	return *this;
}

bool Enemy::IsShooting()
{
	return m_isShooting;
}

void Enemy::TakeHit(int l_damage)
{


	m_hp -= l_damage;

	std::cout << "Enemy took " << l_damage << " damage" << std::endl;


	if (m_hp <= 0)
	{
		m_hp = 0;
		std::cout << "Enemy died" << std::endl;
		alive_ = false;
	}

}

void Enemy::Shoot()
{
	if (!m_isShooting)
	{
		animMgr.SwitchAnimation(AnimType::ShootStand, AnimType::Idle);
		m_isShooting = false;
		m_shootTimer.restart();
	}
}
