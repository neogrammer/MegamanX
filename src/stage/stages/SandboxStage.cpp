#include "SandboxStage.hpp"
#include <iostream>
#include <res/Cfg.hpp>
#include <mgmt/CollisionMgr.hpp>
#include <algorithm>
SandboxStage::SandboxStage()
	: Stage{ StageType::Sandbox }
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
	player_->updateBase(l_dt);
	for (auto& p : projectiles_)
	{
		
		CollisionMgr::CheckCollisions(*p, tilemapSolidTiles_);
	}

	std::vector<std::shared_ptr<ASprite>> tmp;
	tmp.push_back(player_);
	for (auto& p : projectiles_)
	{
		if (!dynamic_cast<Bullet*>(p.get())->GetFriendly())
		{
			CollisionMgr::CheckCollisions(*p, tmp);
		}
	}

	CollisionMgr::CheckCollisions(*player_, tilemapSolidTiles_);

}

void SandboxStage::Render(sf::RenderWindow& l_wnd)
{
	tilemap_->Render(l_wnd);

	for (auto& b : projectiles_)
	{
		b->render(l_wnd);
	}
	player_->render(l_wnd);
}
