#ifndef TILEMAP_HPP__
#define TILEMAP_HPP__
#include <string>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include "Tileset.hpp"
struct Tilemap
{
	void Setup(const std::string& l_tilesetfile, sf::Texture& l_tex, std::string l_filename);
	void Render(sf::RenderWindow& l_wnd);
	void Update(const sf::Time& l_dt);
	bool IsTileSolid(int l_col, int l_row);
	bool IsTileSolid(int l_tileNum);
	std::shared_ptr<Tile> GetTile(int l_tileNum);
	std::shared_ptr<Tile> GetTile(int l_col, int l_row);
	bool IsTileAnimated(int l_col, int l_row);
	bool IsTileAnimated(int l_tileNum);

	std::vector<std::shared_ptr<Tile>>& GetTiles();

private:

	int m_currSetID = { -1 };
	std::vector<std::shared_ptr<Tileset>> m_tilesets = {};
	std::vector<std::shared_ptr<Tile>> m_tiles = {};
	int m_cols = { 0 };
	int m_rows = { 0 };
	sf::Vector2f m_tileSize = {0.f,0.f};
	sf::Texture* m_tex = {nullptr};
};

#endif