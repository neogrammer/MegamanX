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

void AABB::SetVelocity(sf::Vector2f l_vel)
{
	
}

bool AABB::IsNone()
{
	return (localCenter.x == 0.f && localCenter.y == 0.f && halfSize.x == 0.f && halfSize.y == 0.f);
}



AABB BoundingBoxDB::getAABB(ASprite& l_spr)
{
	
	SpriteType l_sType = l_spr.getType();
	SpriteName l_sName = l_spr.getName();
	AnimType l_aType = l_spr.animMgr.getCurrType();

	SpriteKey key = {l_sType, l_sName, l_aType};


	if (bbdbMap.find(key) == bbdbMap.end())
	{	
		// if one not added, return nothing to let outside code know
		return AABB{ {0.f,0.f},{0.f,0.f} };
	}
	else
	{
		// if the AABB for this frame dont exist but the first one does, 
		// fill the ones that do not exist with the first aabb rect
		if (l_spr.animMgr.getCurrFrameIdx() > bbdbMap[key].size())
		{
			int i = (int)bbdbMap[key].size() - 1;
			auto tmp = bbdbMap[key].at(0);
			bbdbMap[key].reserve(l_spr.animMgr.getCurrFrameIdx() + 1);
			while (i < (int)l_spr.animMgr.getCurrFrameIdx())
			{
				bbdbMap[key].push_back(tmp);
				i++;
			}
		}
	}
	// exists now
	return bbdbMap[key].at(l_spr.animMgr.getCurrFrameIdx());
}

AABB BoundingBoxDB::getAABB(ASprite& l_spr, AnimType l_aType, uint32_t l_frameIdx)
{

	SpriteType l_sType = l_spr.getType();
	SpriteName l_sName = l_spr.getName();

	SpriteKey key = { l_sType, l_sName, l_aType };

	if (bbdbMap.find(key) == bbdbMap.end())
	{
		return AABB{ {0.f,0.f},{0.f,0.f} };
	}
	else
	{
		// if improper frame index but exists, return last aabb in the set
		if (l_frameIdx > bbdbMap[key].size())
		{
			return bbdbMap[key].at(bbdbMap[key].size() - 1);
		}
		else
		{
			return bbdbMap[key].at(l_frameIdx);
		}
	}
}


void BoundingBoxDB::addToMap(ASprite& l_spr, AnimType l_aType, uint32_t l_frameIdx, sf::Vector2f l_localCenter, sf::Vector2f l_size)
{
	
	SpriteType l_sType = l_spr.getType();
	SpriteName l_sName = l_spr.getName();
	SpriteKey key = { l_sType, l_sName, l_aType };

	if (l_frameIdx < 0)
	{
		std::cout << "Frame index for bbdbMap[key] is less than zero.  Not added" << std::endl;
		return;
	}
	if (l_frameIdx == 0)
	{
		bbdbMap[key] = std::vector<AABB>{};
		bbdbMap[key].clear();
		bbdbMap[key].emplace_back(AABB{ l_localCenter, l_size });
		return;
	}
	else if (l_frameIdx == bbdbMap[key].size())
	{
		bbdbMap[key].reserve(bbdbMap[key].size() + 1);
		bbdbMap[key].emplace_back(AABB{ l_localCenter, l_size });
		return;
	}
	else
	{
		std::cout << "Frame index for bbdbMap[key] is greater than the size, maybe out of order?  Not added" << std::endl;
		return;
	}
	
}


