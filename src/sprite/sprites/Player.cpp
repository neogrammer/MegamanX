#include "Player.hpp"
#include <res/Cfg.hpp>
#include <iostream>
#include <FSM/duck_fold.hpp>

float Player::JumpForce = -1600.f;
float Player::MoveSpeed = 600.f;
Player::Player(sf::View& l_worldSpace)
	: ASprite{ SpriteType::Actor, SpriteName::Player, Cfg::textures.get((int)Cfg::Textures::PlayerIdle), {{0,0},{136,184}}, {{0,0},{136,184}} }
	, ActionTarget<int>{Cfg::playerInputs}
	, worldView_{l_worldSpace}
{
	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::Idle, 3Ui64, { {0,4*184},{136, 184} },
		3Ui64, 1Ui64, 0.24f, 4.f, true, true, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::Run, 10Ui64, { {0,0},{136, 184} },
		10Ui64, 1Ui64, 0.075f, 0.f, true, true, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::TransRun, 1Ui64, { {0,2*184},{136, 184} },
		1Ui64, 1Ui64, 0.24f, 0.f, true, false, false);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::ShootStand, 2Ui64, { {0,14*184},{136, 184} },
		2Ui64, 1Ui64, 0.14f, 0.04f, true, true, true, true, 0.04f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::TransJump, 2Ui64, { {0,8 * 184},{136, 184} },
		2Ui64, 1Ui64, 0.14f, 0.f, true, false, false);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::Rise, 1Ui64, { {0,12*184},{136, 184} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::TransFall, 1Ui64, { {0,16 * 184}, { 136, 184 } },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, false);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::Fall, 1Ui64, { {0,6 * 184},{136, 184} },
		1Ui64, 1Ui64, 0.14f, 0.f, true, false, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerSheet), AnimLayoutType::Horizontal, AnimType::Land, 2Ui64, { {0,10 * 184},{136, 184} },
		2Ui64, 1Ui64, 0.14f, 0.f, true, false, false);


	spr_.setOrigin({ 68.f, 92.f });
	spr_.setPosition({ 600.f, 496.f - 92.f - 64.f });

	affectedByGravity_ = true;
	grounded_ = false;
	inFreeFall_ = false;
	falling_ = true;
	

	bindActions();
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

	if (jumpHeld_)
	{
		if (GetGrounded())
		{
			justJumped_ = true;
			jumping_ = true;
			SetGrounded(false);
			fallFastFlag_ = false;
			dispatch(fsm, evt_Jumped{});
			vel_.y = -600.f;
		}

		if (jumping_ || inFreeFall_)
		{
			if (fallFastFlag_ == false)
			{
				vel_.y += 1.f;
			}
			else
			{
				vel_.y += 5.f;
			}
		}
	}
	else
	{
		if (jumping_ || inFreeFall_)
		{
			if (fallFastFlag_ == false)
			{
				fallFastFlag_ = true;
			}
			vel_.y += 5.f;
		}
	}


	if (jumping_)
	{
		if (vel_.y >= -40.f)
		{
			jumping_ = false;
			inFreeFall_ = true;
			dispatch(fsm, evt_ReachedJumpPeak{});
		}
	}
	else if (inFreeFall_)
	{
		if (vel_.y >= 0.f)
		{
			inFreeFall_ = false;
			falling_ = true;
			dispatch(fsm, evt_Fell{});
			// gravity will take over from here
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

void Player::SetMoving(bool l_moving, bool l_right)
{
	if (l_right)
	{
		movingRight_ = l_moving;
	}
	else
	{
		movingLeft_ = l_moving;
	}
}

AnimType Player::SyncFSM()
{
	fsm.moving = IsMoving();
	return getAnimType(fsm);
}

sf::View& Player::GetView()
{
	return worldView_;
}

void Player::update(const sf::Time& l_dt)
{

	gameTime_ = l_dt;


	std::cout << collidingOnX_ << std::endl;

	tickPos(l_dt.asSeconds());



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
	if (animMgr.getCurrType() != fsm.getAnimType() && animMgr.getCurrType() != AnimType::Count)
	{
		if (fsm.getAnimType() == AnimType::Land)
		{
			dispatch(fsm, evt_Landed{});
		}
	}

	

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
				if (!pressingLeft_)
				{
					movingRight_ = true;
					facingRight_ = true;
					movingLeft_ = false;
				}
				else
				{
					facingRight_ = false;
					movingLeft_ = false;
					movingRight_ = false;
				}
			
			
			}
			SetGrounded(false);
			dispatch(fsm, evt_StartedMoving{});
			pressingRight_ = true;
			if (!jumping_)
			{
				inFreeFall_ = false;
				falling_ = true;
			}
		});

	bind(Cfg::PlayerInputs::Left, [this](const sf::Event&) {
	
			if (wasFacingRight_)
			{
				movingLeft_ = true;
				movingRight_ = false;
				facingRight_ = false;
				wasFacingRight_ = false;
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

		
			facingRight_ = false;
			movingLeft_ = true;
			movingRight_ = false;
		
			pressingLeft_ = true;
			dispatch(fsm, evt_StartedMoving{});
			SetGrounded(false);
			if (!jumping_)
			{
				inFreeFall_ = false;
				falling_ = true;
			}
		
		});

	bind(Cfg::PlayerInputs::B, [this](const sf::Event&) {
		// Jump Button
		jumpHeld_ = true;
		
			
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

