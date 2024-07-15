#include "Player.hpp"
#include <res/Cfg.hpp>
#include <iostream>

float Player::JumpForce = -1600.f;
float Player::MoveSpeed = 600.f;
Player::Player()
	: ASprite{ SpriteType::Actor, Cfg::textures.get((int)Cfg::Textures::PlayerIdle) }
	, ActionTarget<int>{Cfg::playerInputs}
{
	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerIdle), AnimLayoutType::Horizontal, AnimType::Idle, 3Ui64, { {0,0},{120,136} },
		3Ui64, 1Ui64, 0.24f, 0.f, true, true, true, true, 5.f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerRun), AnimLayoutType::Horizontal, AnimType::Run, 10Ui64, { {0,0},{136,140} },
		10Ui64, 1Ui64, 0.075f, 0.f, true, true, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerTransRun), AnimLayoutType::Horizontal, AnimType::TransRun, 1Ui64, { {0,0},{135,136} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, false);

	spr_.setOrigin({ 60.f, 68.f });
	spr_.setPosition({ 600.f, 496.f - 68.f - 64.f });

	affectedByGravity_ = true;
	grounded_ = false;

	

	bindActions();
}


Player::Player(const Player& other)
	: ASprite(other)
	, ActionTarget<int>{Cfg::playerInputs}
{
	bindActions();
}

Player& Player::operator=(const Player& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	return *this;
}

void Player::processInput()
{
	movingRight_ = false;
	movingLeft_ = false;
	jumpHeld_ = false;
	ActionTarget<int>::processEvents();
}

bool Player::IsMoving()
{
	if (movingRight_ || movingLeft_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Player::update(const sf::Time& l_dt)
{
	// apply gravity
	if (affectedByGravity_)
	{
		if (!grounded_)
		{
			if (!jumpHeld_ && jumpLetGo_ && vel_.y < -40.f)
			{
				vel_.y = -40.f;
				jumpLetGo_ = false;
			}

			vel_.y += Cfg::Gravity * l_dt.asSeconds();
		}



	}
	// apply input force, if any
	if (!movingLeft_ && !movingRight_)
	{
		vel_.x = 0.f;
		// if (grounded_)
		animMgr.SwitchAnimation(AnimType::Idle);
	}
	else
	{
		
		if (movingLeft_)
		{
			vel_.x = -Player::MoveSpeed;
		
			/*if (grounded_)
				SetGrounded(false);*/

			animMgr.SwitchAnimation(AnimType::TransRun, AnimType::Run);
		}
		if (movingRight_)
		{
			vel_.x = Player::MoveSpeed;
		
			/*if (grounded_)
				SetGrounded(false);*/

			animMgr.SwitchAnimation(AnimType::TransRun, AnimType::Run);
		}
	}
	animMgr.SetFacingRight(facingRight_);

	gameTime_ = l_dt;
	animMgr.Update(l_dt);
}

void Player::bindActions()
{
	bind(Cfg::PlayerInputs::Up, [this](const sf::Event&) {
		
		
		});

	bind(Cfg::PlayerInputs::Right, [this](const sf::Event&) {
		if (movingLeft_)
		{
			movingLeft_ = false;
		}
		else
		{
			movingRight_ = true;
			facingRight_ = true;
		}
		});

	bind(Cfg::PlayerInputs::Left, [this](const sf::Event&) {
		if (movingRight_)
		{
			movingRight_ = false;
		}
		else
		{
			movingLeft_ = true;
			facingRight_ = false;


		}
		});

	bind(Cfg::PlayerInputs::B, [this](const sf::Event&) {
		jumpHeld_ = true;
		
		if (grounded_)
		{
			grounded_ = false;
			jumpLetGo_ = true;
			vel_.y = Player::JumpForce;
		}
		});
}

