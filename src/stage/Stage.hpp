#ifndef STAGE_HPP__
#define STAGE_HPP__
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <misc/globals.hpp>
#include <vector>
#include <sprite/sprites/Bullet.hpp>
#include <sprite/sprites/Tile.hpp>
#include <sprite/sprites/Player.hpp>
#include <map/Tilemap.hpp>

struct Stage
{
	Stage() = delete;
	explicit Stage(StageType l_type);
	virtual ~Stage() = default;

	Stage(const Stage&) = delete;
	Stage& operator=(const Stage&) = delete;
	Stage(Stage&&) = delete;
	Stage& operator=(Stage&&) = delete;

	virtual void ProcessInput() = 0;
	virtual void HandleEvent(const sf::Event& l_e) = 0;
	virtual void Update(const sf::Time& l_dt) = 0;
	virtual void Render(sf::RenderWindow& l_wnd) = 0;

	StageType getType();

protected:

	StageType m_type;

	std::vector<std::shared_ptr<ASprite>> projectiles_;
	std::vector<std::shared_ptr<ASprite>> tiles_;
	std::vector<Tilemap> tilemap_;
	std::shared_ptr<ASprite> player_;


};

#endif