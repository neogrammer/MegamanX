#ifndef TILE_HPP__
#define TILE_HPP__
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <sprite/ASprite.hpp>

struct Tile : ASprite
{
	friend struct Tileset;
	friend struct Tilemap;
	Tile() = default;
	Tile(sf::Texture& l_tex, sf::IntRect l_startFrame = { {0,0},{0,0} }, sf::IntRect l_bbox = { {0,0},{0,0} });
	~Tile() override = default;

	Tile(const Tile&);
	Tile& operator=(const Tile&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;

	int getSetID();
	int getTileID();

	bool IsSolid();
	bool IsAnimated();

	void SetSolid(bool l_solid);
	void SetAnimated(bool l_animated);


private:
	sf::Sprite m_spr{};

	int m_setID{ -1 };
	int m_tileID{ -1 };

	bool m_solid{ true };
	bool m_animated{ false };

	
};

#endif