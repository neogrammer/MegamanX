#include "aabb.hpp"
#include <sprite/ASprite.hpp>
vf2d::vf2d()
	: sf::Vector2f{0.f,0.f}
{

}

vf2d::vf2d(float x, float y)
	: sf::Vector2f{ x, y }
{

}

rect::rect()
	: pos{ 0.f,0.f }
	, size{ 0.f,0.f }
	, vel{0.f,0.f}
	, spr{nullptr}
{

}

rect::rect(ASprite* l_spr)
	: spr{ l_spr }
	, pos{0.f,0.f}
	, size{0.f,0.f}
	, vel{0.f,0.f}
{
	Reset();
}

void rect::Reset()
{
	if (spr == nullptr)
	{
		pos = { 0.f, 0.f };
		size = { 0.f, 0.f };
		vel = { 0.f,0.f };
	}
	else
	{ 
		pos = {(*spr)().getGlobalBounds().left - ((*spr)().getGlobalBounds().width / 2.f) + (*spr)().getOrigin().x,
		(*spr)().getGlobalBounds().top - ((*spr)().getGlobalBounds().height / 2.f) + (*spr)().getOrigin().y };
		size = { (float)(*spr)().getTextureRect().width, (float)(*spr)().getTextureRect().height};
		vel = { spr->vel().x, spr->vel().y };
	}
}
