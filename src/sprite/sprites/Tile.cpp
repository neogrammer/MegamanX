#include "Tile.hpp"
#include <res/Cfg.hpp>

Tile::Tile()
	: ASprite{}
{

}

Tile::Tile(sf::Texture& l_tex)
	: ASprite{ SpriteType::Tile, SpriteName::NormalTile, l_tex}
{
	facingRight_ = true;
	animMgr.SetFacingRight(true);

}


Tile::Tile(const Tile& other)
	: ASprite(other)
	, m_setID{other.m_setID}
	, m_tileID{other.m_tileID}
{

}

Tile& Tile::operator=(const Tile& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	this->m_setID = other.m_setID;
	this->m_tileID = other.m_tileID;
	return *this;
}

void Tile::processInput()
{

}


void Tile::update(const sf::Time& l_dt)
{
	gameTime_ = l_dt;

}

int Tile::getSetID()
{
	return m_setID;
}

int Tile::getTileID()
{
	return m_tileID;
}

bool Tile::IsSolid()
{
	return m_solid;
}

bool Tile::IsAnimated()
{
	return m_animated;
}

void Tile::SetSolid(bool l_solid)
{
	m_solid = l_solid;
}

void Tile::SetAnimated(bool l_animated)
{
	m_animated = l_animated;
}

