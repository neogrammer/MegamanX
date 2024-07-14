#include "Tilemap.hpp"
#include <fstream>
void Tilemap::Setup(sf::Texture& l_tex, std::string l_filename)
{

	std::ifstream iFile;
	iFile.open(l_filename);

	if (!iFile.is_open())
		return;

	m_tex = &l_tex;
	m_currSetID += 1;

	m_tilesets.clear();
	m_tilesets.emplace_back(std::make_shared<Tileset>());


	int sheetTiles, sheetCols, sheetRows, tileW, tileH;
	iFile >> sheetTiles >> sheetCols >> sheetRows >> tileW >> tileH;
	m_tilesets.back()->setup(l_tex, sheetTiles, sheetCols, sheetRows, { tileW, tileH });
	m_tileSize = {(float)tileW, (float)tileH};

	iFile >> m_cols >> m_rows;
	m_tiles.clear();
	m_tiles.reserve(m_cols * m_rows);
	for (int y = 0; y < m_rows; y++)
	{
		for (int x = 0; x < m_cols; x++)
		{
			int tileSheetTileNum;
			iFile >> tileSheetTileNum;
			m_tiles.emplace_back(std::make_shared<Tile>(m_tilesets.at(0)->copyTile(tileSheetTileNum)));
			(*m_tiles.back())().setTextureRect(m_tilesets.at(0)->GetRect(tileSheetTileNum));
			(*m_tiles.back())().setOrigin({ tileW / 2.f, tileH / 2.f });
			(*m_tiles.back())().setPosition({ (float)(x * tileW), (float)(y * tileH) });
		}
	}


}

void Tilemap::Render(sf::RenderWindow& l_wnd)
{
	for (auto& t : m_tiles)
	{
		auto tmp = (*dynamic_cast<ASprite*>(t.get()))().getOrigin();
		(*dynamic_cast<ASprite*>(t.get()))().setOrigin({ 0.f,0.f });
		dynamic_cast<ASprite*>(t.get())->render(l_wnd);
		(*dynamic_cast<ASprite*>(t.get()))().setOrigin(tmp);
	}
}

void Tilemap::Update(const sf::Time& l_dt)
{
	for (auto& t : m_tiles)
	{
		dynamic_cast<ASprite*>(t.get())->updateBase(l_dt);
	}
}

bool Tilemap::IsTileSolid(int l_col, int l_row)
{
	int tilenum = l_row * m_cols + l_col;
	return m_tiles.at(tilenum)->IsSolid();
}

bool Tilemap::IsTileSolid(int l_tileNum)
{
	return m_tiles.at(l_tileNum)->IsSolid();
}

std::shared_ptr<Tile> Tilemap::GetTile(int l_tileNum)
{
	return m_tiles.at(l_tileNum);
}

std::shared_ptr<Tile> Tilemap::GetTile(int l_col, int l_row)
{
	int tilenum = l_row * m_cols + l_col;
	return m_tiles.at(tilenum);
}

bool Tilemap::IsTileAnimated(int l_col, int l_row)
{
	int tilenum = l_row * m_cols + l_col;
	return m_tiles.at(tilenum)->IsAnimated();
}

bool Tilemap::IsTileAnimated(int l_tileNum)
{
	return m_tiles.at(l_tileNum)->IsAnimated();

}
