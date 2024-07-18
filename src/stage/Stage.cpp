#include "Stage.hpp"
#include <memory>
#include <utility>
#include <res/Cfg.hpp>

Stage::Stage(StageType l_type)
	: m_type{ l_type }
	, projectiles_{}
	, player_{}
	, tilemap_{}
	, enemies_{}
	, tilemapSolidTiles_{}
	, worldSpace_{}
{
	worldSpace_ = sf::View({ 800.f, 450.f }, { 1600.f,900.f });
	projectiles_.clear();
	tilemapSolidTiles_.clear();
	enemies_.clear();
}

StageType Stage::getType()
{
	return m_type;
}
void Stage::CreateFriendlyBullet(ASprite& l_spr, SpriteName l_name)
{
	sf::Vector2f pos = { (!l_spr.IsFacingRight()) ? l_spr().getPosition().x - l_spr().getOrigin().x + 10.f : l_spr().getPosition().x + l_spr().getOrigin().x - 10.f, l_spr().getPosition().y - l_spr().getOrigin().y + ((float)l_spr().getTextureRect().height / 3.f) + 18.f };

	switch (l_name)
	{
	case SpriteName::BusterBullet:
	{
		projectiles_.reserve(projectiles_.size() + 1);
		projectiles_.emplace_back(std::make_shared<Bullet>(Cfg::textures.get((int)Cfg::Textures::Bullet1)));
		(*projectiles_.back())().setTextureRect({ { 0, 0 }, { 24 , 18 } });
		(*projectiles_.back())().setOrigin({ 12.f, 9.f });
		(*projectiles_.back()).vel() = { (l_spr.IsFacingRight()) ? 600.f : -600.f, 0.f };
		(*projectiles_.back())().setPosition(pos);
	}
		break;
	default:
		break;
	}
}
