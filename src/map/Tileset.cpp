#include "Tileset.hpp"

#include <fstream>
#include <res/Cfg.hpp>
int Tileset::m_numTilesets = 0;


void Tileset::setup(const std::string& l_tilesetFile, sf::Texture& l_tex, int l_numTiles, int l_numCols, int l_numRows, sf::Vector2i l_tileSize)
{
	m_tex = &l_tex;
	m_setID = m_numTilesets++;
	m_tiles.clear();
	m_tiles.reserve(l_numTiles);

	for (int y = 0; y < l_numRows; y++)
	{
		for (int x = 0; x < l_numCols; x++)
		{
			int tilenum = y * l_numCols + x;
			if (tilenum >= l_numTiles)
				break;
			m_tiles.emplace_back(std::make_shared<Tile>(l_tex));
			(*m_tiles.back())().setTextureRect({ {(int)(x * l_tileSize.x),(int)(y * l_tileSize.y)}, {(int)l_tileSize.x, (int)l_tileSize.y}});
			(*m_tiles.back())().setOrigin({ l_tileSize.x / 2.f, l_tileSize.y / 2.f });
			(*m_tiles.back())().setPosition({ 0.f, 0.f });
			m_tiles.back()->m_setID = 0;
			m_tiles.back()->m_tileID = tilenum;
		}
	}

	std::ifstream iFile;
	iFile.open(l_tilesetFile);
	if (!iFile.is_open()) return;
	for (int y = 0; y < l_numRows; y++)
	{
		for (int x = 0; x < l_numCols; x++)
		{
			int tilenum = y * l_numCols + x;
			if (tilenum >= l_numTiles)
				break;
			int solid;
			iFile >> solid;
			m_tiles.at(tilenum)->SetSolid((solid == 1) ? true : false);
		}
	}

	for (int y = 0; y < l_numRows; y++)
	{
		for (int x = 0; x < l_numCols; x++)
		{
			int tilenum = y * l_numCols + x;
			if (tilenum >= l_numTiles)
				break;
			int solid;
			iFile >> solid;
			m_tiles.at(tilenum)->SetAnimated((solid == 1) ? true : false);
		}
	}

}

Tile Tileset::copyTile(int tileID)
{
	return *m_tiles.at(tileID);
}

sf::IntRect Tileset::GetRect(int tileID)
{
	return m_tiles.at(tileID)->GetTextureRect();
}
