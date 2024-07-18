#include "BuzzBird.hpp"
#include <res/Cfg.hpp>
#include <iostream>


BuzzBird::BuzzBird() : Enemy{ SpriteName::BuzzBird, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw) }
{
	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw), AnimLayoutType::Horizontal, AnimType::Idle, 7Ui64, { {0,0},{220,296} },
		7Ui64, 1Ui64, 0.16f, 0.f, false, true, true, true, 4.f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw), AnimLayoutType::Horizontal, AnimType::ShootStand, 9Ui64, { {0,296},{220,296} },
		9Ui64, 1Ui64, 0.075f, 0.f, false, false, false);

	spr_.setOrigin({ 110.f, 148.f });
	spr_.setPosition({ 1600.f - 64.f - 128.f, 900.f - 148.f - (1.f*64.f) });
	affectedByGravity_ = false;

	animMgr.SwitchAnimation(AnimType::Idle);

	for (int i = 0; i < 7; i++)
	{
		Cfg::bboxDB.addToMap(*this, AnimType::Idle, i, { 112.f, 100.f }, { 24.f, 174.f });
	}

	for (int i = 0; i < 9; i++)
	{
		Cfg::bboxDB.addToMap(*this, AnimType::ShootStand, i, { 112.f, 100.f }, { 24.f, 174.f });
	}

}

BuzzBird::BuzzBird(const BuzzBird& other)
	: Enemy{ SpriteName::BuzzBird, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw) }
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw), AnimLayoutType::Horizontal, AnimType::Idle, 7Ui64, { {0,0},{220,296} },
		7Ui64, 1Ui64, 0.16f, 0.f, false, true, true, true, 4.f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw), AnimLayoutType::Horizontal, AnimType::ShootStand, 9Ui64, { {0,296},{220,296} },
		9Ui64, 1Ui64, 0.075f, 0.f, false, false, false);

	spr_.setOrigin({ 110.f, 148.f });
	spr_.setPosition(other.spr_.getPosition());
	affectedByGravity_ = false;

	animMgr.SwitchAnimation(AnimType::Idle);


}

BuzzBird& BuzzBird::operator=(const BuzzBird& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw), AnimLayoutType::Horizontal, AnimType::Idle, 7Ui64, { {0,0},{220,296} },
		7Ui64, 1Ui64, 0.16f, 0.f, false, true, true, true, 4.f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::BirdSheetWSaw), AnimLayoutType::Horizontal, AnimType::ShootStand, 9Ui64, { {0,296},{220,296} },
		9Ui64, 1Ui64, 0.075f, 0.f, false, false, false);

	spr_.setOrigin({ 110.f, 148.f });
	spr_.setPosition(other.spr_.getPosition());
	affectedByGravity_ = false;

	animMgr.SwitchAnimation(AnimType::Idle);



	return *this;
}

void BuzzBird::processInput()
{
}

void BuzzBird::update(const sf::Time& l_dt)
{
	gameTime_ = l_dt;

	if (invincible_)
	{
		invincibleTime_ += gameTime_;
		if (invincibleTime_ > sf::seconds(3.f))
		{
			invincible_ = false;
		}
	}

	if (IsShooting())
	{
		if (m_shootTimer.getElapsedTime().asSeconds() > 5.f)
		{
			m_isShooting = false;
		}
	}

	animMgr.Update(l_dt);
}
