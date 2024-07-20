#include <misc/globals.hpp>
#include <sprite/sprites/Projectile.hpp>
#include <sprite/sprites/Tile.hpp>
#include <sprite/sprites/Player.hpp>



ASprite::ASprite(SpriteType l_type, SpriteName l_name, sf::Texture& l_tex, sf::IntRect l_startFrame)
	: type_{l_type}
	, name_{l_name}
	, spr_{}
	, vel_{}
	, gameTime_{g::FPS60}
	, animMgr{}
	, isSetExternal{}
{
	spr_.setTexture(l_tex);
	if (l_startFrame.width != 0 && l_startFrame.height != 0)
		spr_.setTextureRect(l_startFrame);
	spr_.setPosition(0.f, 0.f);


	animMgr.AddAnimation(this->spr_, l_tex, AnimLayoutType::Count, AnimType::Count, 1, spr_.getTextureRect(), 1, 1, 0.f, false, true, true, true);
	
}

sf::Sprite& ASprite::operator()()
{
	return spr_;
}

bool ASprite::IsFacingRight()
{
    return facingRight_;
}


ASprite::ASprite(const ASprite& other)
	: type_{ other.type_ }
	, name_{ other.name_ }
	, spr_{other.spr_ }
	, vel_{other.vel_ }
	, gameTime_{ other.gameTime_ }
	, animMgr{}
	, isSetExternal{other.isSetExternal}
{
	if (&other == this)
	{
		return;
	}
	spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	(*const_cast<AnimationMgr*>(&other.animMgr)).copyDataTo(this->animMgr, this->spr_);


}

ASprite& ASprite::operator=(const ASprite& other)
{

	if (&other == this)
	{
		return *this;
	}
	

	this->spr_ = sf::Sprite{};
	this->spr_ = other.spr_;
	this->type_ = other.type_;
	this->name_ = other.name_;
	this->vel_ = other.vel_;
	this->gameTime_ = other.gameTime_;
	this->isSetExternal = other.isSetExternal;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	(*const_cast<AnimationMgr*>(&other.animMgr)).copyDataTo(this->animMgr, this->spr_);

	return *this;
}

void ASprite::render(sf::RenderWindow& l_wnd)
{
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

const sf::Sprite& ASprite::getSpr() const
{
	return spr_;
}

const sf::Vector2f& ASprite::getVel() const
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

sf::IntRect ASprite::GetTextureRect()
{
    return this->spr_.getTextureRect();
}

void ASprite::SetGrounded(bool l_grounded)
{
	grounded_ = l_grounded;
	
}

bool ASprite::GetGrounded()
{
	return grounded_;
}

void ASprite::setFixedPosition(sf::Vector2f l_pos)
{
	isSetExternal = true;
	spr_.setPosition(l_pos);
}

bool ASprite::JustJumped()
{
	return justJumped_;
}

void ASprite::SetJustJumped(bool l_jumped)
{
	justJumped_ = l_jumped;
}



bool ASprite::NeedsStageToShoot()
{
	return needsStageToShoot_;
}

void ASprite::BulletWasShot()
{
	needsStageToShoot_ = false;
}

void ASprite::BulletWasDestroyed()
{
	if (numBullets_ > 0)
		numBullets_--;
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

bool ASprite::IsShooting()
{
	return shooting_;
}

void ASprite::Shoot()
{
	if (!shooting_)
	{
		shooting_ = true;
	}
}

void ASprite::updatePosition()
{
	if (!isSetExternal)
	{
		spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
	}
}


void ASprite::setVelocity(const sf::Vector2f& l_vel)
{
	vel_.x = l_vel.x;
	vel_.y = l_vel.y;
}

void ASprite::accelerate(sf::Vector2f l_acceleration, float l_dt)
{
	vel_.x += l_acceleration.x * l_dt;
	vel_.y += l_acceleration.y * l_dt;
}

void ASprite::tickPos(float l_dt)
{
	this->spr_.move({vel_.x * l_dt, vel_.y * l_dt});
}

void ASprite::setPos(sf::Vector2f l_pos)
{
	this->spr_.setPosition(l_pos);
}

void ASprite::move(sf::Vector2f l_offset, float l_dt)
{
	this->spr_.move(l_offset * l_dt);
}

const float ASprite::bTop() const
{
	return this->spr_.getPosition().y - this->spr_.getOrigin().y + (float)this->getCurrBoxRect().top;
}

const float ASprite::bLeft() const
{
	return this->spr_.getPosition().x - this->spr_.getOrigin().x + (float)this->getCurrBoxRect().left;

}

const float ASprite::bRight() const
{
	return this->spr_.getPosition().x - this->spr_.getOrigin().x + (float)this->getCurrBoxRect().left + (float)this->getCurrBoxRect().width;

}

const float ASprite::bBottom() const
{
	return this->spr_.getPosition().y - this->spr_.getOrigin().y + (float)this->getCurrBoxRect().top + (float)this->getCurrBoxRect().height;


}

const sf::Vector2f ASprite::boxTL() const
{
	return {bTop(),bLeft()};
}

const sf::Vector2f ASprite::boxBR() const
{
	return {bRight(),bBottom()};
}

sf::FloatRect ASprite::getBoxGlobalBounds()
{
	sf::FloatRect tmp = {};

	sf::IntRect bRect = animMgr.getCurrBox();
	tmp.left = this->spr_.getPosition().x - this->spr_.getOrigin().x + (float)bRect.left;
	tmp.top = this->spr_.getPosition().y - this->spr_.getOrigin().y + (float)bRect.top;
	tmp.width = (float)bRect.width;
	tmp.height = (float)bRect.height;

	return tmp;
}

void ASprite::setBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight, const sf::IntRect& l_rect)
{
	animMgr.setBoxRect(l_type, l_index, l_facingRight, l_rect);
}

sf::IntRect ASprite::getBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight)
{
	return animMgr.getBoxRect(l_type, l_index, l_facingRight);
}

const sf::IntRect& ASprite::getCurrBoxRect() const
{
	return animMgr.getCurrBox();
}



sf::FloatRect GetSweptBroadphaseBox(ASprite& l_spr1, float l_dt)
{
	struct r
	{
		float x, y, w, h, vx, vy;
	};

	r b{ l_spr1.bLeft(),l_spr1.bTop(), (float)l_spr1.getCurrBoxRect().width,  (float)l_spr1.getCurrBoxRect().height, l_spr1.vel().x,l_spr1.vel().y };

	sf::FloatRect broadphasebox = l_spr1.getBoxGlobalBounds();

	broadphasebox.left = b.x;
	broadphasebox.top = b.y;
	broadphasebox.width = b.vx > 0 ? b.vx * l_dt + b.w : b.w - b.vx * l_dt;
	broadphasebox.height = b.vy > 0 ? b.vy * l_dt + b.h : b.h - b.vy * l_dt;

	return broadphasebox;
}


bool BroadphaseCheck(sf::FloatRect l_spr1, ASprite& l_spr2)
{
	auto b1 = l_spr1;
	auto b2 = l_spr2.getBoxGlobalBounds();
	return !(b1.left + b1.width < l_spr2.bLeft() || b1.left > l_spr2.bRight() || b1.top + b1.height < l_spr2.bTop() || b1.top > l_spr2.bBottom());
}