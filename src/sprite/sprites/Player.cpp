#include "Player.hpp"
#include <res/Cfg.hpp>
#include <iostream>
#include <FSM/duck_fold.hpp>

float Player::JumpForce = -1600.f;
float Player::MoveSpeed = 600.f;
Player::Player()
	: ASprite{ SpriteType::Actor, SpriteName::Player, Cfg::textures.get((int)Cfg::Textures::PlayerIdle) }
	, ActionTarget<int>{Cfg::playerInputs}
{
	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerIdle), AnimLayoutType::Horizontal, AnimType::Idle, 3Ui64, { {0,0},{120,136} },
		3Ui64, 1Ui64, 0.24f, 0.f, true, true, true, true, 5.f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerRun), AnimLayoutType::Horizontal, AnimType::Run, 10Ui64, { {0,0},{136,140} },
		10Ui64, 1Ui64, 0.075f, 0.f, true, true, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerTransRun), AnimLayoutType::Horizontal, AnimType::TransRun, 1Ui64, { {0,0},{135,136} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, false);


	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerShootStand), AnimLayoutType::Horizontal, AnimType::ShootStand, 2Ui64, { {0,0},{120,136} },
		2Ui64, 1Ui64, 0.14f, 0.04f, true, true, true, true, 0.04);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerTransJump), AnimLayoutType::Horizontal, AnimType::TransJump, 2Ui64, { {0,0},{120,184} },
		2Ui64, 1Ui64, 0.14f, 0.f, true, false, false);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerRise), AnimLayoutType::Horizontal, AnimType::Rise, 1Ui64, { {0,0},{120,184} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerTransFall), AnimLayoutType::Horizontal, AnimType::TransFall, 1Ui64, { {0,0},{120,184} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, false);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerFall), AnimLayoutType::Horizontal, AnimType::Fall, 1Ui64, { {0,0},{120,184} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerLand), AnimLayoutType::Horizontal, AnimType::Land, 2Ui64, { {0,0},{120,184} },
		2Ui64, 1Ui64, 0.14f, 0.f, true, false, false);


	spr_.setOrigin({ 60.f, 92.f });
	spr_.setPosition({ 600.f, 496.f - 92.f - 64.f });

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
	pressingLeft_ = false;
	pressingShoot_ = false;
	pressingRight_ = false;
	movingRight_ = false;
	movingLeft_ = false;
	jumpHeld_ = false;
	ActionTarget<int>::processEvents();

	if (pressingShoot_ == false)
	{
		dispatch(fsm, evt_StoppedShooting{});
	}

	if (pressingRight_ == true && pressingLeft_ == true)
	{
		movingRight_ = false;
		movingLeft_ = false;

		if (wasFacingRight_)
		{
			facingRight_ = true;
		}
		else
		{
			facingRight_ = false;
		}
	}

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

bool Player::IsShooting()
{
	return shooting_;
}

void Player::Shoot()
{

	if (!shooting_ && numBullets_ < maxBullets_)
	{
		shooting_ = true;
		shootElapsed_ = sf::Time::Zero;
		needsStageToShoot_ = true;
		numBullets_++;
	}
	else
	{
		shootElapsed_ += gameTime_;
		if (shootElapsed_ > shootDelay_)
		{
			shooting_ = false;
		}
	}
}

void Player::TakeHit(int l_damage)
{
	// NOT YET IMPLEMENTED
	std::cout << "Player took " << l_damage << " damage" << std::endl;
}

AnimType Player::SyncFSM()
{
	fsm.moving = IsMoving();
	return getAnimType(fsm);
}

void Player::update(const sf::Time& l_dt)
{

	gameTime_ = l_dt;

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

			

			if (vel_.y > 0.f && fsm.getAnimType() != AnimType::TransFall && fsm.getAnimType() != AnimType::Fall)
			{
				dispatch(fsm, evt_ReachedJumpPeak{});
			}

			vel_.y += Cfg::Gravity * l_dt.asSeconds();
		}
		else
		{
			if (fsm.getAnimType() == AnimType::Rise || fsm.getAnimType() == AnimType::Fall || fsm.getAnimType() == AnimType::TransJump || fsm.getAnimType() == AnimType::TransFall)
			{
				dispatch(fsm, evt_ReachedJumpPeak{}, evt_Landed{});
			}
		}




	}
	// apply input force, if any
	if (!movingLeft_ && !movingRight_)
	{
		vel_.x = 0.f;
		// if (grounded_)
		//animMgr.SwitchAnimation(AnimType::Idle);
		dispatch(fsm, evt_StoppedMoving{});
	}
	else
	{
		
		if (movingLeft_)
		{
			vel_.x = -Player::MoveSpeed;
		
			/*if (grounded_)
				SetGrounded(false);*/

			//animMgr.SwitchAnimation(AnimType::TransRun, AnimType::Run);
			dispatch(fsm, evt_StartedMoving{});
		}
		if (movingRight_)
		{
			vel_.x = Player::MoveSpeed;
		
			/*if (grounded_)
				SetGrounded(false);*/

			//animMgr.SwitchAnimation(AnimType::TransRun, AnimType::Run);
			dispatch(fsm, evt_StartedMoving{});
		}
	}
	animMgr.SetFacingRight(facingRight_);



	// must be in a valid animatable state, thus a always is not Count
	AnimType a = SyncFSM();

	

	switch (a)
	{
	case AnimType::Idle:
	case AnimType::Run:
	case AnimType::Rise:
	case AnimType::Fall:
	case AnimType::ShootStand:
		animMgr.SwitchAnimation(a);
		break;
	case AnimType::TransRun:
		animMgr.SwitchAnimation(a, AnimType::Run);
		break;
	case AnimType::TransJump:
		animMgr.SwitchAnimation(a, AnimType::Rise);
		break;
	case AnimType::TransFall:
		animMgr.SwitchAnimation(a, AnimType::Fall);
		break;
	case AnimType::Land:
		animMgr.SwitchAnimation(a, AnimType::Idle);
		break;
	default:
		break;
	}

	

	if (animMgr.currentIsOnLastFrame() && animMgr.currIsReadyToPop() && animMgr.currFallback() != nullptr)
	{
		if (animMgr.currFallback()->GetType() == AnimType::Rise)
		{
			dispatch(fsm, evt_TransToRiseCompleted{});
		}
		else if (animMgr.currFallback()->GetType() == AnimType::Fall)
		{
			dispatch(fsm, evt_TransToFallingCompleted{});
		}
		else if (animMgr.currFallback()->GetType() == AnimType::Idle)
		{
			dispatch(fsm, evt_LandingTransCompleted{});
		}
	}

	animMgr.Update(l_dt);

	if (animMgr.getCurrType() != fsm.getAnimType() && animMgr.getCurrType() != AnimType::Count)
	{
		if (fsm.getAnimType() == AnimType::ShootStand)
		{
			dispatch(fsm, evt_StoppedShooting{});
		}
	}
	if (animMgr.getCurrType() != fsm.getAnimType())
	{
		if (fsm.getAnimType() == AnimType::Land)
		{
			dispatch(fsm, evt_Landed{});
		}
	}

	ASprite::SyncSpriteToAnim(*this);
}

void Player::bindActions()
{
	bind(Cfg::PlayerInputs::Up, [this](const sf::Event&) {
		
		
		});

	bind(Cfg::PlayerInputs::Right, [this](const sf::Event&) {
			if (wasFacingRight_)
			{
				movingRight_ = true;
				movingLeft_ = false;
				facingRight_ = true;
			}
			else if (movingLeft_ && !wasFacingRight_)
			{
				if (!pressingLeft_)
				{
					facingRight_ = true;
					movingLeft_ = false;
					movingRight_ = true;
				}
				else
				{
					facingRight_ = false;
					movingLeft_ = false;
					movingRight_ = false;
				}
			}
			else
			{
				facingRight_ = false;
				movingLeft_ = false;
				movingRight_ = true;
				//wasFacingRight_ = true;
			}
		
			pressingRight_ = true;
		});

	bind(Cfg::PlayerInputs::Left, [this](const sf::Event&) {

			if (!wasFacingRight_)
			{
				movingLeft_ = true;
				movingRight_ = false;
				facingRight_ = false;
			}
			else if (movingRight_ && wasFacingRight_)
			{
				if (!pressingLeft_)
				{
					facingRight_ = false;
					movingLeft_ = true;
					movingRight_ = false;
				}
				else
				{
					facingRight_ = false;
					movingLeft_ = false;
					movingRight_ = false;
				}
			}
			else
			{
				facingRight_ = true;
				movingLeft_ = true;
				movingRight_ = false;
				wasFacingRight_ = false;
			}

			if (!pressingRight_)
				facingRight_ = false;

			pressingLeft_ = true;
		
		});

	bind(Cfg::PlayerInputs::B, [this](const sf::Event&) {
		jumpHeld_ = true;
		
		if (grounded_)
		{
			grounded_ = false;
			jumpLetGo_ = true;
			vel_.y = Player::JumpForce;
			dispatch(fsm, evt_Jumped{});
		}
		});


	bind(Cfg::PlayerInputs::Y, [this](const sf::Event&) {
		
		Shoot();
		pressingShoot_ = true;
		//if (shooting_ == true && shootElapsed_ == sf::Time::Zero)
		//{
			dispatch(fsm, evt_StartedShooting{});
		//}
		});
}

