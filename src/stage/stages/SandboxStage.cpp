#include "SandboxStage.hpp"
#include <iostream>
#include <res/Cfg.hpp>
#include <mgmt/CollisionMgr.hpp>
#include <algorithm>
SandboxStage::SandboxStage()
	: Stage{ StageType::Sandbox }
{

	tiles_.reserve(2);



	tiles_.emplace_back(std::make_shared<Tile>(Cfg::textures.get((int)Cfg::Textures::Tileset1)));
	(*tiles_.back())().setTextureRect({{0, 0}, {64 , 64}});
	(*tiles_.back())().setOrigin({ 32.f, 32.f });
	(*tiles_.back())().setPosition({ 1600.f - 32.f, 450.f });

	tiles_.emplace_back(std::make_shared<Tile>(Cfg::textures.get((int)Cfg::Textures::Tileset1)));
	(*tiles_.back())().setTextureRect({ {0, 0}, {64 , 64} });
	(*tiles_.back())().setOrigin({ 32.f, 32.f });
	(*tiles_.back())().setPosition({ 32.f, 900.f - 32.f });

	tilemap_.emplace_back(Tilemap{});
	tilemap_.back().Setup(Cfg::textures.get((int)Cfg::Textures::Tileset1), "assets/data/tilemap/tilemap1.dat");

	projectiles_.reserve(3);
	projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
	(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 18 , 18 } });
	(*projectiles_.back())().setOrigin({ 9.f, 9.f });
	(*projectiles_.back()).vel() = { 600.f, 0.f };
	(*projectiles_.back())().setPosition({ 0.f, 450.f});

	projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
	(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 18 , 18 } });
	(*projectiles_.back())().setOrigin({ 9.f, 9.f });
	(*projectiles_.back()).vel() = { 600.f, 0.f };
	(*projectiles_.back())().setPosition({ 40.f, 450.f }); 
	
	projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
	(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 18 , 18 } });
	(*projectiles_.back())().setOrigin({ 9.f, 9.f });
	(*projectiles_.back()).vel() = { 600.f, 0.f };
	(*projectiles_.back())().setPosition({ 80.f, 450.f }); 

	player_ = std::make_shared<Player>();
}

void SandboxStage::ProcessInput()
{
	for (auto& s : tiles_)
	{
		s->processInputBase();
	}
	for (auto& b : projectiles_)
	{
		b->processInputBase();
	}
}

void SandboxStage::HandleEvent(const sf::Event& l_e)
{
}

void SandboxStage::Update(const sf::Time& l_dt)
{
	projectiles_.erase(std::remove_if(projectiles_.begin(), projectiles_.end(), [&](auto& p) ->bool { return !p->IsAlive(); }), projectiles_.end());


	for (auto& s : tiles_)
	{
		s->updateBase(l_dt);
	}
	for (auto& b : projectiles_)
	{
		b->updateBase(l_dt);
	}
	tilemap_[0].Update(l_dt);
	player_->updateBase(l_dt);
	for (auto& p : projectiles_)
	{
		CollisionMgr::CheckCollisions(*p, tiles_);
	}

	std::vector<std::shared_ptr<ASprite>> tmp;
	tmp.push_back(player_);
	for (auto& p : projectiles_)
	{
		CollisionMgr::CheckCollisions(*p, tmp);
	}

	CollisionMgr::CheckCollisions(*player_, tiles_);

	
}

void SandboxStage::Render(sf::RenderWindow& l_wnd)
{
	tilemap_[0].Render(l_wnd);

	for (auto& t : tiles_)
	{
		t->render(l_wnd);
	}
	for (auto& b : projectiles_)
	{
		b->render(l_wnd);
	}
	player_->render(l_wnd);
}
