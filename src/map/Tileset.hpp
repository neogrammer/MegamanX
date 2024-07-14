#ifndef TILESET_HPP__
#define TILESET_HPP__
#include <sprite/sprites/Tile.hpp>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <utility>
#include <string>
struct Tileset
{
	void setup(sf::Texture& l_tex, int l_numTiles, int l_numCols, int l_numRows, sf::Vector2i l_tileSize);

	Tile copyTile(int tileID);
	sf::IntRect GetRect(int tileID);
private:
	
	sf::Texture* m_tex = {};
	std::vector<std::shared_ptr<Tile>> m_tiles = {};

	int m_setID{ -1 };
	static int m_numTilesets;
};

#endif