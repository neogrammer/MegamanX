#include "Player.hpp"
#include <res/Cfg.hpp>
Player::Player()
	: ASprite{ SpriteType::Actor, Cfg::textures.get((int)Cfg::Textures::PlayerIdle) }
{
	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerIdle), AnimLayoutType::Horizontal, AnimType::Idle, 3Ui64, { {0,0},{120,136} },
		3Ui64, 1Ui64, 0.24f, 0.f, true, true, true, true, 5.f);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerRun), AnimLayoutType::Horizontal, AnimType::Run, 10Ui64, { {0,0},{136,140} },
		10Ui64, 1Ui64, 0.24f, 0.f, true, true, true);

	animMgr.AddAnimation(spr_, Cfg::textures.get((int)Cfg::Textures::PlayerTransRun), AnimLayoutType::Horizontal, AnimType::TransRun, 1Ui64, { {0,0},{135,136} },
		1Ui64, 1Ui64, 0.24f, 0.f, true, false, false);

	spr_.setOrigin({ 60.f, 68.f });
	spr_.setPosition({ 60.f,900.f - 68.f - 64.f });
}


Player::Player(const Player& other)
	: ASprite(other)
{
}

Player& Player::operator=(const Player& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	return *this;
}

void Player::processInput()
{

}


void Player::update(const sf::Time& l_dt)
{
	gameTime_ = l_dt;
	animMgr.Update(l_dt);
}

