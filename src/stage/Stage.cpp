#include "Stage.hpp"

Stage::Stage(StageType l_type)
	: m_type{ l_type }
	, projectiles_{}
	, tiles_{}
	, player_{}
{
	projectiles_.clear();
	tiles_.clear();
}

StageType Stage::getType()
{
	return m_type;
}
