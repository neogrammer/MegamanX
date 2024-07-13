#ifndef COLLISIONMGR_HPP__
#define COLLISIONMGR_HPP__
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <memory>
#include <utility>
struct ASprite;
struct CollisionMgr
{
	CollisionMgr() = delete;
	CollisionMgr(const CollisionMgr&) = delete;
	CollisionMgr& operator=(const CollisionMgr&) = delete;

	static void CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec);
	static bool IsColliding(ASprite& l_sprA, ASprite& l_sprB);
	static void ResolveActor(ASprite& l_spr);
	static void ResolveProjectile(ASprite& l_sprA, ASprite& l_sprB);

private:
	static sf::FloatRect m_currOverlap;
	static sf::FloatRect m_prevOverlap;

};

#endif