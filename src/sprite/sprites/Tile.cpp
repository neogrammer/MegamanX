#include "Tile.hpp"

Tile::Tile()
	: ASprite{}
{

}

Tile::Tile(sf::Texture& l_tex)
	: ASprite{ SpriteType::Tile, l_tex }
{

}


Tile::Tile(const Tile& other)
	: ASprite(other)
{
}

Tile& Tile::operator=(const Tile& other)
{
	this->spr_ = other.spr_;
	this->spr_.setTexture(*const_cast<sf::Texture*>(other.spr_.getTexture()));
	return *this;
}

void Tile::processInput()
{

}


void Tile::update(const sf::Time& l_dt)
{
	gameTime_ = l_dt;

}

