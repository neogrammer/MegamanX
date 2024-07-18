#ifndef COLLISIONMGR_HPP__
#define COLLISIONMGR_HPP__
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <memory>
#include <utility>
#include <misc/aabb.hpp>
#include <SFML/System/Time.hpp>

struct ASprite;

struct CollisionMgr
{
	CollisionMgr() = delete;
	CollisionMgr(const CollisionMgr&) = delete;
	CollisionMgr& operator=(const CollisionMgr&) = delete;

	static bool CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec, sf::Vector2f& cp, sf::Vector2f& cn, float& t, const sf::Time& l_dt);
	static bool IsColliding(ASprite& l_sprA, ASprite& l_sprB);
	static void ResolveActor(ASprite& l_spr);
	static void ResolveProjectile(ASprite& l_sprA, ASprite& l_sprB);

	static bool PointVsRect(const sf::Vector2f& l_p, ASprite& l_spr);
	static bool RectVsRect(ASprite& l_spr1, ASprite& l_spr2);
	static bool RayVsRect(const sf::Vector2f& l_ray_origin, const sf::Vector2f& l_ray_dir, rect& target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& t_hit_near);
	static bool DynamicRectVsRect(ASprite& l_in, ASprite& l_target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& l_contact_time, const sf::Time& fElapsedTime);
	static bool DynamicRectVsRect2(ASprite& l_in, ASprite& l_target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& l_contact_time, const sf::Time& fElapsedTime);

private:
	static sf::FloatRect m_currOverlap;
	static sf::FloatRect m_prevOverlap;

};

#endif