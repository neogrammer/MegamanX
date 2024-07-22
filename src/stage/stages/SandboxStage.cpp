#include "SandboxStage.hpp"
#include <iostream>
#include <res/Cfg.hpp>
#include <mgmt/CollisionMgr.hpp>
#include <algorithm>
#include <FSM/duck_fold.hpp>
SandboxStage::SandboxStage()
	: Stage{ StageType::Sandbox }
	, pointOfContact_{}
	, boundingBox_{}
	, mpos{}
{

	tilemap_ = std::make_shared<Tilemap>();
	tilemap_->Setup("assets/data/tileset/tileset1.dat", Cfg::textures.get((int)Cfg::Textures::Tileset1), "assets/data/tilemap/tilemap1.dat");
	tilemapSolidTiles_.clear();
	tilemapSolidTiles_.reserve(tilemap_->GetTiles().size());
	for (int i = 0; i < tilemap_->GetTiles().size(); i++)
	{
		if (tilemap_->IsTileSolid(i))
			tilemapSolidTiles_.push_back(tilemap_->GetTile(i));
	}
	
	player_ = std::make_shared<Player>(worldSpace_);

	pointOfContact_.setRadius(10.f);
	pointOfContact_.setFillColor(sf::Color::Red);
	pointOfContact_.setOrigin({ 5.0f,5.0f });
	pointOfContact_.setPosition({-10.f,-10.f});

	boundingBox_.setSize({ (sf::Vector2f)player_->GetTextureRect().getSize() });
	boundingBox_.setFillColor(sf::Color(255,0,0,110));
	boundingBox_.setOrigin({ (*player_)().getOrigin() });
	boundingBox_.setPosition({ -1.f * (*player_)().getOrigin().x, -1.f * (*player_)().getOrigin().y });

	enemies_.emplace_back(std::make_shared<BuzzBird>());

	player_->SetGrounded(false);
	dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Fell{});

	castline[0].position.x = startPointLine.x;
	castline[0].position.y = startPointLine.y;
	castline[1].position.x = (float)mpos.x;
	castline[1].position.y = (float)mpos.y;
	castline[0].color = sf::Color::Red;
	castline[1].color = sf::Color::Red;

	normLine[0].position.x = 0.f;
	normLine[0].position.y = 0.f;
	normLine[1].position.x = 1.f;
	normLine[1].position.y = 1.f;
	normLine[0].color = sf::Color::Yellow;
	normLine[1].color = sf::Color::Yellow;

}

SandboxStage::~SandboxStage()
{}

void SandboxStage::ProcessInput()
{
	// not sure why i have this yet, but allow inputtable changes to the tiles on the map
	// maybe for doors or chests?
	for (auto& s : tilemap_->GetTiles())
	{
		s->processInputBase();
	}
	// bullets may be controllable
	// handled here
	for (auto& b : projectiles_)
	{
		b->processInputBase();
	}
	// handle user's input to affect the player
	player_->processInputBase();
}

void SandboxStage::HandleEvent(const sf::Event& l_e)
{
}

void SandboxStage::Update(const sf::Time& l_dt)
{
	if (player_->NeedsStageToShoot())
	{
		CreateFriendlyBullet(*player_, SpriteName::BusterBullet);
		player_->BulletWasShot();
	}

	// erase any dead bullets here from last frame
	projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(), [&](auto& p) ->bool{
		if (!p->IsAlive()){	if (dynamic_cast<Projectile*>(p.get())->GetFriendly()) { player_->BulletWasDestroyed(); } } 
		return !p->IsAlive(); }), projectiles_.end());

	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [&](auto& e) ->bool { return !e->IsAlive(); }), enemies_.end());

	std::vector<std::shared_ptr<ASprite>> tmp = {};
	tmp.clear();
	tmp.reserve(enemies_.size() + projectiles_.size() + tilemapSolidTiles_.size() + 1);
	for (auto& s : enemies_) { tmp.push_back(s); }
	for (auto& s : tilemapSolidTiles_){	tmp.push_back(s); }
	for (auto& s : projectiles_) { 	tmp.push_back(s); }
	tmp.push_back(player_);



	// enemies
	// update bullet animations and positions
	for (auto& b : projectiles_) {
		b->updateBase(l_dt); if ((*b)().getPosition().x > 1600.f || (*b)().getPosition().x < 0.f) { b->SetAlive(false); }
		for (auto& e : enemies_)
		{
			if (CollisionMgr::RectVsRect(*b, *e))
			{
				dynamic_cast<Enemy*>(e.get())->TakeHit(dynamic_cast<Projectile*>(b.get())->GetDamage());
				b->SetAlive(false);
			}
		}
	}
	// enemies
	for (auto& e : enemies_) { e->updateBase(l_dt); }



	HandlePlayerMovement(l_dt);




	// these variables are for debugging
	boundingBox_.setPosition((*player_)().getPosition());
	boundingBox_.setSize((sf::Vector2f)(*player_)().getTextureRect().getSize());
	boundingBox_.setOrigin((*player_)().getOrigin());

	bgOffset += 10.f * l_dt.asSeconds();
	
}

void SandboxStage::Render(sf::RenderWindow& l_wnd)
{



	l_wnd.setView(worldSpace_);
	
	

	sf::Sprite bg{};
	bg.setTexture(Cfg::textures.get((int)Cfg::Textures::BGSpace));
	bg.move({ bgOffset,0.f });

	auto currLeft = worldSpace_.getCenter().x - 800.f;
	if (bgOffset >= currLeft)
	{
		//create another background size.x to the left of the bgOffset
		sf::Sprite bgLeft{};
		bgLeft.setTexture(Cfg::textures.get((int)Cfg::Textures::BGSpace));
		bgLeft.setPosition({ bgOffset - bgLeft.getTexture()->getSize().x, 0.f });

		l_wnd.draw(bgLeft);
		if (bgLeft.getPosition().x >= currLeft)
		{
			sf::Sprite bgLeftLeft{};
			bgLeftLeft.setTexture(Cfg::textures.get((int)Cfg::Textures::BGSpace));
			bgLeftLeft.setPosition({ bgLeft.getPosition().x - bgLeft.getTexture()->getSize().x, 0.0f });
			l_wnd.draw(bgLeftLeft);
			if (bgLeftLeft.getPosition().x >= currLeft)
			{
				bgOffset = 0.0f;
				bg.setPosition({ currLeft, 0.f });

			}

		}
	}

	l_wnd.draw(bg);

	// draw map
	tilemap_->Render(l_wnd);

	// draw enemies
	for (auto& e : enemies_) { e->render(l_wnd); }

	// draw the player
	player_->render(l_wnd);



	// draw bullets
	for (auto& b : projectiles_) { b->render(l_wnd); }

	//l_wnd.draw(castline);

	// For debugging purposes
	//l_wnd.draw(boundingBox_);
	//l_wnd.draw(castline);
	//if (playerColliding)
	//{
		//l_wnd.draw(pointOfContact_);
		//l_wnd.draw(normLine);
	//}

	


}