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
	projectiles_.reserve(3);
	projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
	(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 18 , 18 } });
	(*projectiles_.back())().setOrigin({ 9.f, 9.f });
	(*projectiles_.back()).vel() = { 600.f, 0.f };
	(*projectiles_.back())().setPosition({ 0.f, 896.f - 68.f - 64.f - 10.f});

	projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
	(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 18 , 18 } });
	(*projectiles_.back())().setOrigin({ 9.f, 9.f });
	(*projectiles_.back()).vel() = { 600.f, 0.f };
	(*projectiles_.back())().setPosition({ 40.f, 896.f - 68.f - 64.f - 10.f });
	
	projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
	(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 18 , 18 } });
	(*projectiles_.back())().setOrigin({ 9.f, 9.f });
	(*projectiles_.back()).vel() = { 600.f, 0.f };
	(*projectiles_.back())().setPosition({ 80.f, 896.f - 68.f - 64.f - 10.f });

	player_ = std::make_shared<Player>();

	pointOfContact_.setRadius(10.f);
	pointOfContact_.setFillColor(sf::Color::Red);
	pointOfContact_.setOrigin({ 5.0f,5.0f });
	pointOfContact_.setPosition({-10.f,-10.f});

	boundingBox_.setSize({ (sf::Vector2f)player_->GetRect().getSize() });
	boundingBox_.setFillColor(sf::Color(255,0,0,110));
	boundingBox_.setOrigin({ (*player_)().getOrigin() });
	boundingBox_.setPosition({ -1.f * (*player_)().getOrigin().x, -1.f * (*player_)().getOrigin().y });

	enemies_.emplace_back(std::make_shared<BuzzBird>());
}

SandboxStage::~SandboxStage()
{
	
}

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

	// update the player's bounding box size for the collision checks
	//ASprite::SyncSpriteToAnim(*player_);

	// put player into a vector for collision checking
	std::vector<std::shared_ptr<ASprite>> tmp{};
	tmp.clear();
	tmp.push_back(player_);


	// collision checking out param variables
	sf::Vector2f cp, cn;
	float t;

	// control flags used to determine implementation details
	bool collided = false;

	// erase any dead bullets here from last frame
	projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(), [&](auto& p) ->bool { return !p->IsAlive(); }), projectiles_.end());
	enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [&](auto& e) ->bool { return !e->IsAlive(); }), enemies_.end());

	//update any animated tiles
	for (auto& s : tilemap_->GetTiles()) { s->updateBase(l_dt); }

	// update bullet animations and positions
	for (auto& b : projectiles_) { b->updateBase(l_dt);	}
	
	// handle bullets to tiles collisions
	for (auto& p : projectiles_) { if (CollisionMgr::CheckCollisions(*p, tilemapSolidTiles_, cp, cn, t, l_dt) && t <= 1.f) {int fillerJob = 0;} }

	// handle bullets to enemy collisions
	for (auto& p : projectiles_) { if (CollisionMgr::CheckCollisions(*p, enemies_, cp, cn, t, l_dt) && t <= 1.f) { int fillerJob = 0; } }

	// enemies
	for (auto& e : enemies_) { e->updateBase(l_dt); }

	// handle bullets to player collisions
	for (auto& p : projectiles_) 
	{ 
		// only if bullets arent the player's bullets
		if (!dynamic_cast<Bullet*>(p.get())->GetFriendly())
		{
			if (CollisionMgr::CheckCollisions(*p, tmp, cp, cn, t, l_dt)) { int fillerJob = 0; }
		}
	}

	// enemies


	// main handling of player adjustment for gravity and movement colliding against the map tiles
	if (CollisionMgr::CheckCollisions(*player_, tilemapSolidTiles_, cp, cn, t, l_dt))
	{
		pointOfContact_.setPosition(cp);
		collided = true;

	}
	// update the player after handling the collision checking
	player_->updateBase(l_dt);
	

	// after updating the player's position, if there was a collision, adjust further for gravity and running into walls
	// hacky, leave as is
	if (collided)
	{
		if ((*player_)().getPosition().y + (*player_)().getOrigin().y >= cp.y)
		{
			(*player_)().setPosition((*player_)().getPosition().x, cp.y - (*player_)().getOrigin().y);
			player_->SetGrounded(true);
			dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Landed{});
		}
		else
		{
			player_->SetGrounded(false);
		}
	}
	else
	{
		// if no collision occurred, player may be standing on ground, but if moving, set grounded to false to checks can be made again
		// and the player doesn't float midair
		if (dynamic_cast<Player*>(player_.get())->IsMoving())
		{
			player_->SetGrounded(false);

			dispatch(dynamic_cast<Player*>(player_.get())->fsm, evt_Fell{});
		}
	}
	//if (collided)
		//(*player_)().setPosition({ (*player_)().getPosition().x, cp.y - (*player_)().getOrigin().y });

	// these variables are for debugging
	boundingBox_.setPosition((*player_)().getPosition());
	boundingBox_.setSize((sf::Vector2f)(*player_)().getTextureRect().getSize());
	boundingBox_.setOrigin((*player_)().getOrigin());

	
	
}

void SandboxStage::Render(sf::RenderWindow& l_wnd)
{
	// draw map
	tilemap_->Render(l_wnd);

	// draw enemies
	for (auto& e : enemies_) { e->render(l_wnd); }

	// draw the player
	player_->render(l_wnd);



	// draw bullets
	for (auto& b : projectiles_) { b->render(l_wnd); }

	// For debugging purposes
	//l_wnd.draw(boundingBox_);
	//l_wnd.draw(pointOfContact_);
}
