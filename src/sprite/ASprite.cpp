#include <misc/globals.hpp>
#include <sprite/sprites/Bullet.hpp>
#include <sprite/sprites/Tile.hpp>


ASprite::ASprite(SpriteType l_type, sf::Texture& l_tex)
	: type_{l_type}
	, spr_{}
	, vel_{}
	, gameTime_{g::FPS60}
{
	spr_.setTexture(l_tex);
}

sf::Sprite& ASprite::operator()()
{
	return spr_;
}


ASprite::ASprite(const ASprite& other)
{
	if (&other == this)
	{
		return;
	}
	type_ = other.type_;
	spr_ = other.spr_;
	vel_ = other.vel_;
	gameTime_ = other.gameTime_;
	spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
}

ASprite& ASprite::operator=(const ASprite& other)
{
	this->spr_ = sf::Sprite{};
	this->type_ = other.type_;
	this->vel_ = { 0.f, 0.f };
	this->gameTime_ = sf::Time::Zero;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	return *this;
}

void ASprite::render(sf::RenderWindow& l_wnd)
{
	// set texture rect to correct animation rect
	updateTexRect();
	// adjust position by velocity
	updatePosition();
	l_wnd.draw(spr_);
}

sf::Vector2f& ASprite::vel()
{
	return vel_;
}

SpriteType ASprite::getType()
{
	return type_;
}

bool ASprite::IsAlive()
{
	return alive_;
}

void ASprite::SetAlive(bool l_alive)
{
	alive_ = l_alive;
}

sf::IntRect ASprite::GetRect()
{
    return this->spr_.getTextureRect();
}

void ASprite::processInputBase()
{
	processInput();
}

void ASprite::updateBase(const sf::Time& l_dt)
{
	//gameTime_ = l_dt;
	update(l_dt);
}

void ASprite::updatePosition()
{
	spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
}

void ASprite::updateTexRect()
{
	// not implemented
}
