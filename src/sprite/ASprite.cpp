#include <misc/globals.hpp>
#include <sprite/sprites/Projectile.hpp>
#include <sprite/sprites/Tile.hpp>
#include <sprite/sprites/Player.hpp>



ASprite::ASprite(SpriteType l_type, SpriteName l_name, sf::Texture& l_tex, sf::IntRect l_startFrame, sf::Vector2f l_boxSize, sf::Vector2f l_boxOffset)
	: type_{l_type}
	, name_{l_name}
	, spr_{}
	, vel_{}
	, gameTime_{g::FPS60}
	, boxMap{}
{
	spr_.setTexture(l_tex);
	if (l_startFrame.width != 0 && l_startFrame.height != 0)
		spr_.setTextureRect(l_startFrame);
	spr_.setPosition(0.f, 0.f);

	boxMap.clear();
	boxMap.insert(std::pair( AnimType::None, std::vector<std::shared_ptr<Box>>{} ));
	boxMap.at(AnimType::None).clear();
	boxMap.at(AnimType::None).reserve(1);
	boxMap.at(AnimType::None).emplace_back(std::make_unique<Box>());
	
	boxMap.at(AnimType::None).back()->pSpr = this;

	boxMap.at(AnimType::None).back()->sprXPos = const_cast<float*>(&spr_.getPosition().x);
	boxMap.at(AnimType::None).back()->sprYPos = const_cast<float*>(&spr_.getPosition().y);

	boxMap.at(AnimType::None).back()->x = l_boxOffset.x;
	boxMap.at(AnimType::None).back()->y = l_boxOffset.y;

	sf::Vector2f sz{};
	if (l_boxSize == sf::Vector2f{ 0.f, 0.f })
	{
		sf::Vector2f sz = sf::Vector2f(spr_.getTextureRect().getSize());
	}
	else
	{ 
		sz = l_boxSize;
	}
	
	boxMap.at(AnimType::None).back()->w = sz.x;
	boxMap.at(AnimType::None).back()->h = sz.y;
	boxMap.at(AnimType::None).back()->vx = &vel_.x;
	boxMap.at(AnimType::None).back()->vy = &vel_.y;
	boxMap.at(AnimType::None).back()->isBroadphaseBox = false;
	//invariant setup

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
	boxMap.clear();
	boxMap[AnimType::None] = std::vector<std::shared_ptr<Box>>();
	boxMap[AnimType::None].clear();
	boxMap[AnimType::None].push_back(other.boxMap.at(AnimType::None).at(0));

}

ASprite& ASprite::operator=(const ASprite& other)
{
	this->spr_ = sf::Sprite{};
	this->type_ = other.type_;
	this->vel_ = { 0.f, 0.f };
	this->gameTime_ = sf::Time::Zero;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	this->boxMap.clear();
	this->boxMap[AnimType::None] = std::vector<std::shared_ptr<Box>>();
	boxMap[AnimType::None].clear();
	boxMap[AnimType::None].push_back(other.boxMap.at(AnimType::None).at(0));

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
    return false;
}

void ASprite::Shoot()
{
}

void ASprite::updatePosition()
{
	if (!isSetExternal)
	{
		
		/*
		if (this->type_ == SpriteType::Actor)
		{
			if (facingRight_ && this->spr_.getPosition().x + vel_.x * gameTime_.asSeconds() > dynamic_cast<Player*>(this)->GetView().getCenter().x)
			{
				auto diff = ((this->spr_.getPosition().x + vel_.x * gameTime_.asSeconds()) - dynamic_cast<Player*>(this)->GetView().getCenter().x);
				spr_.setPosition(dynamic_cast<Player*>(this)->GetView().getCenter().x, spr_.getPosition().y + vel_.y * gameTime_.asSeconds());
				dynamic_cast<Player*>(this)->GetView().move({ diff, 0.f });
			}
			else if (!facingRight_ && this->spr_.getPosition().x + vel_.x * gameTime_.asSeconds() < dynamic_cast<Player*>(this)->GetView().getCenter().x)		
			{
				auto diff = (dynamic_cast<Player*>(this)->GetView().getCenter().x - (this->spr_.getPosition().x + vel_.x * gameTime_.asSeconds()));
				if (dynamic_cast<Player*>(this)->GetView().getCenter().x - 800.f - diff >= 0.f)
				{
					spr_.setPosition(dynamic_cast<Player*>(this)->GetView().getCenter().x, spr_.getPosition().y + vel_.y * gameTime_.asSeconds());
					dynamic_cast<Player*>(this)->GetView().move({ -diff, 0.f });
				}
				else
				{
					if (spr_.getPosition().x - spr_.getOrigin().x + vel_.x * gameTime_.asSeconds() > 32.f)
						spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });

				}
			}
			else
			{
				if (!facingRight_)
				{
					if (spr_.getPosition().x - spr_.getOrigin().x + vel_.x * gameTime_.asSeconds() >= abs(vel_.x * gameTime_.asSeconds()) + 32.f)
					{
						spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
					}
				
				
				}
				else
				{
					spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
				}
		
			}*/
		//}
		//else
		//{
			spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
		}
		//spr_.move({ vel_.x * gameTime_.asSeconds(),vel_.y * gameTime_.asSeconds() });
	
}


void ASprite::updateTexRect()
{
	// not implemented
	oldHeight = (float)GetRect().height;
}
