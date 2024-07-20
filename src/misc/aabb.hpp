#ifndef AABB_HPP__
#define AABB_HPP__
#include <SFML/System/Vector2.hpp>
#include <map>
#include "globals.hpp"
#include <vector>
#include <compare>
struct ASprite;
struct vf2d
{
	vf2d();
	vf2d(float l_x, float l_y);
	~vf2d() = default;
	vf2d(const vf2d&) = default;
	vf2d& operator=(const vf2d&) = default;
	vf2d(vf2d&&) = default;
	vf2d& operator=(vf2d&&) = default;

	float x{ 0 }, y{ 0 };

};


struct rect
{
	rect();
	rect(ASprite* l_spr);
	~rect() = default;
	rect(const rect&) = default;
	rect& operator=(const rect&) = default;
	rect(rect&&) = delete;
	rect& operator=(rect&&) = delete;
	ASprite* spr{ nullptr };
	vf2d pos;
	vf2d size;
	vf2d vel;

	void Reset();
};

struct AABB
{
	AABB() = default;
	AABB(sf::Vector2f l_center, sf::Vector2f l_size);
	~AABB() = default;
	AABB(const AABB&) = default;
	AABB& operator=(const AABB&) = default;

	AABB(AABB&&) = default;
	AABB& operator=(AABB&&) = default;

	vf2d TopLeftFromCenter();
	vf2d BottomRightFromCenter();
	vf2d LocalCenter();
	vf2d GetSize();
	vf2d GetHalfSize();
	void SetVelocity(sf::Vector2f l_vel);
	bool IsNone();

private:
	vf2d halfSize;
	vf2d localCenter;

	
};



// Define comparison operators for SpriteKey (optional, but recommended)

struct BoundingBoxDB
{
	BoundingBoxDB() = default;
	~BoundingBoxDB() = default;

	BoundingBoxDB(const BoundingBoxDB&) = default;
	BoundingBoxDB& operator=(const BoundingBoxDB&) = default;

	BoundingBoxDB(BoundingBoxDB&&) = default;
	BoundingBoxDB& operator=(BoundingBoxDB&&) = default;


	AABB getAABB(ASprite& l_spr);
	AABB getAABB(ASprite& l_spr, AnimType l_aType, uint32_t l_frameIndex);


	void addToMap(ASprite& l_spr, AnimType l_aType, uint32_t l_frameIndex, sf::Vector2f l_localCenter, sf::Vector2f l_size);

	

private:
	std::map<SpriteKey, std::vector<AABB>> bbdbMap{};
};




#endif