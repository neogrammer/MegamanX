#include "Bullet.hpp"

Bullet::Bullet()
	: ASprite{}
{

}

Bullet::Bullet(sf::Texture& l_tex)
	: ASprite{SpriteType::Projectile, l_tex}
{
	animMgr.AddAnimation(spr_, l_tex, AnimLayoutType::Horizontal, AnimType::Idle, 5Ui64, { {0,0},{24,16} },
		5Ui64, 1Ui64, 0.003f, 0.f, false, true, true);
}


Bullet::Bullet(const Bullet& other)
	: ASprite(other)
{
}

Bullet& Bullet::operator=(const Bullet& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
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

