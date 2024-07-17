#ifndef AABB_HPP__
#define AABB_HPP__
#include <SFML/System/Vector2.hpp>
#include <map>
#include "globals.hpp"
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

private:
	vf2d halfSize;
	vf2d localCenter;

	
};

struct SpriteKey {
	SpriteType type;
	SpriteName name;
	AnimType animType;

	//bool operator==(const SpriteKey& rhs);

	SpriteKey() = default;
	SpriteKey(SpriteType l_spriteType, SpriteName l_spriteName, AnimType l_animType)
		: type{l_spriteType}
		, name{ l_spriteName }
		, animType{l_animType}
	{}

	~SpriteKey() = default;

	SpriteKey(const SpriteKey&) = default;
	SpriteKey& operator=(const SpriteKey&) = default;

	SpriteKey(SpriteKey&&) = default;
	SpriteKey& operator=(SpriteKey&&) = default;

	// Define the spaceship operator
	auto operator<=>(const SpriteKey& other) const = default;
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

	void addToMap(ASprite& l_spr, sf::Vector2f l_localCenter, sf::Vector2f l_size);

	

private:
	std::map<SpriteKey, AABB> bbdbMap{};
};




#endif