#include "aabb.hpp"
#include <sprite/ASprite.hpp>
#include <iostream>
#include <type_traits>
#include <res/Cfg.hpp>
vf2d::vf2d()
	: x { 0.f }
	, y { 0.f }
{

}

vf2d::vf2d(float l_x, float l_y)
	: x{ l_x }
	, y{ l_y }
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

AABB::AABB(sf::Vector2f l_center, sf::Vector2f l_size)
	: localCenter{l_center.x, l_center.y}
	, halfSize{ l_size.x / 2.f, l_size.y / 2.f }
{}

vf2d AABB::TopLeftFromCenter()
{
	return vf2d{ localCenter.x - halfSize.x,  localCenter.y - halfSize.y };
}

vf2d AABB::BottomRightFromCenter()
{
    return vf2d{ localCenter.x + halfSize.x,  localCenter.y + halfSize.y };
}

vf2d AABB::LocalCenter()
{
    return localCenter;
}

vf2d AABB::GetSize()
{
	return vf2d{halfSize.x * 2.f,halfSize.y * 2.f};
}

vf2d AABB::GetHalfSize()
{
    return halfSize;
}



AABB BoundingBoxDB::getAABB(ASprite& l_spr)
{
	
	SpriteType l_sType = l_spr.getType();
	SpriteName l_sName = l_spr.getName();
	AnimType l_aType = l_spr.animMgr.getCurrType();

	SpriteKey key = {l_sType, l_sName, l_aType};

	if (bbdbMap.find(key) == bbdbMap.end())
	{
		return AABB{ {0.f,0.f},{0.f,0.f} };
	}
	return bbdbMap[key];
}


void BoundingBoxDB::addToMap(ASprite& l_spr, sf::Vector2f l_localCenter, sf::Vector2f l_size)
{
	
	SpriteType l_sType = l_spr.getType();
	SpriteName l_sName = l_spr.getName();
	AnimType l_aType = l_spr.animMgr.getCurrType();
	SpriteKey key = { l_sType, l_sName, l_aType };

	bbdbMap[key] = AABB(l_localCenter, l_size);
}

