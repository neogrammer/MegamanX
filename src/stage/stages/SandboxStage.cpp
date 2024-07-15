#include "SandboxStage.hpp"
#include <iostream>
#include <res/Cfg.hpp>
#include <mgmt/CollisionMgr.hpp>
#include <algorithm>
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
}

SandboxStage::~SandboxStage()
{
	
}

void SandboxStage::ProcessInput()
{
	for (auto& s : tilemap_->GetTiles())
	{
		s->processInputBase();
	}
	for (auto& b : projectiles_)
	{
		b->processInputBase();
	}
	player_->processInputBase();
	
}

void SandboxStage::HandleEvent(const sf::Event& l_e)
{
}

void SandboxStage::Update(const sf::Time& l_dt)
{
	projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(), [&](auto& p) ->bool { return !p->IsAlive(); }), projectiles_.end());


	for (auto& s : tilemap_->GetTiles())
	{
		s->updateBase(l_dt);
	}


	for (auto& b : projectiles_)
	{
		b->updateBase(l_dt);
	}
	

	sf::Vector2f cp;
	sf::Vector2f cn;
	float t;

	for (auto& p : projectiles_)
	{

		if (CollisionMgr::CheckCollisions(*p, tilemapSolidTiles_, cp, cn, t, l_dt))
		{
			std::cout << "Bullet collided with a tile" << std::endl;

		}
	}

	std::vector<std::shared_ptr<ASprite>> tmp;
	tmp.push_back(player_);
	for (auto& p : projectiles_)
	{
		if (!dynamic_cast<Bullet*>(p.get())->GetFriendly())
		{
			if (CollisionMgr::CheckCollisions(*p, tmp, cp, cn, t, l_dt))
			{
				std::cout << "Unfriendly Bullet collided with Player" << std::endl;

			}
		}
	}

	bool collided = false;
	if (CollisionMgr::CheckCollisions(*player_, tilemapSolidTiles_, cp, cn, t, l_dt))
	{
		boundingBox_.setPosition((*player_)().getPosition());
		pointOfContact_.setPosition(cp);
		collided = true;
		std::cout << "Player Bullet collided with a/some tile(s)" << std::endl;
	}

	player_->updateBase(l_dt);

	if (collided)
	{
		if ((*player_)().getPosition().y + (*player_)().getOrigin().y > cp.y && cn.x == 0.f)
		{
			(*player_)().setPosition((*player_)().getPosition().x, cp.y - (*player_)().getOrigin().y);
			player_->SetGrounded(true);
		}
	}
	else
	{
		if (dynamic_cast<Player*>(player_.get())->IsMoving())
		{
			player_->SetGrounded(false);
		}
	}
}

void SandboxStage::Render(sf::RenderWindow& l_wnd)
{
	tilemap_->Render(l_wnd);

	for (auto& b : projectiles_)
	{
		b->render(l_wnd);
	}
	player_->render(l_wnd);
	l_wnd.draw(boundingBox_);
	l_wnd.draw(pointOfContact_);
}
