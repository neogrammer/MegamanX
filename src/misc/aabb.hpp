#ifndef AABB_HPP__
#define AABB_HPP__
#include <SFML/System/Vector2.hpp>
struct ASprite;
struct vf2d : public sf::Vector2f
{
	vf2d();
	vf2d(float x, float y);
	~vf2d() = default;
	vf2d(const vf2d&) = default;
	vf2d& operator=(const vf2d&) = default;
	vf2d(vf2d&&) = default;
	vf2d& operator=(vf2d&&) = default;
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

#endif