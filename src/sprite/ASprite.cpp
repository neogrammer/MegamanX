#include <misc/globals.hpp>
#include <sprite/sprites/Projectile.hpp>
#include <sprite/sprites/Tile.hpp>
#include <sprite/sprites/Player.hpp>



ASprite::ASprite(SpriteType l_type, SpriteName l_name, sf::Texture& l_tex)
	: type_{l_type}
	, name_{l_name}
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

	if (movingRight_)
		wasFacingRight_ = true;
	if (movingLeft_)
		wasFacingRight_ = false;
}

sf::Vector2f& ASprite::vel()
{
	return vel_;
}

SpriteType ASprite::getType()
{
	return type_;
}

SpriteName ASprite::getName()
{
	return name_;
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

void ASprite::SetGrounded(bool l_grounded)
{
	grounded_ = l_grounded;
	
}

void ASprite::setFixedPosition(sf::Vector2f l_pos)
{
	isSetExternal = true;
	spr_.setPosition(l_pos);
}

void ASprite::SyncSpriteToAnim(ASprite& l_spr)
{

	float currOriginX = l_spr().getOrigin().x;
	float currOriginY = l_spr().getOrigin().y;

	float newOriginX = l_spr().getTextureRect().width / 2.f;
	float newOriginY = l_spr().getTextureRect().height / 2.f;

	float yOriginDiff = abs(newOriginY - currOriginY);

	l_spr().setOrigin({ newOriginX, newOriginY });

	float newHeight = (float)l_spr.GetRect().height;

	float newYOffset = abs(newHeight - l_spr.oldHeight);

	if (l_spr.oldHeight < newHeight)
	{
		l_spr().setPosition({ l_spr().getPosition().x, l_spr().getPosition().y + (-1.f * newYOffset) + yOriginDiff});
	}
	else if (l_spr.oldHeight > newHeight)
	{
		l_spr().setPosition({ l_spr().getPosition().x, l_spr().getPosition().y + newYOffset - yOriginDiff });
	}
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
	if (!isSetExternal)
		spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
}

void ASprite::updateTexRect()
{
	// not implemented
	oldHeight = (float)GetRect().height;
}
