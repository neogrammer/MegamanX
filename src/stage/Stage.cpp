#include "Stage.hpp"

Stage::Stage(StageType l_type)
	: m_type{ l_type }
	, projectiles_{}
	, player_{}
	, tilemap_{}
	, tilemapSolidTiles_{}
{
	projectiles_.clear();
	tilemapSolidTiles_.clear();
}

StageType Stage::getType()
{
	return m_type;
}
