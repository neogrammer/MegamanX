#ifndef COLLISIONMGR_HPP__
#define COLLISIONMGR_HPP__
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <memory>
#include <utility>
#include <SFML/System/Time.hpp>
#include <res/Cfg.hpp>
#include <misc/globals.hpp>
struct ASprite;
struct Player;
struct Tile;
struct BuzzSaw;
struct Bullet;

struct CollisionMgr
{
	CollisionMgr() = delete;
	CollisionMgr(const CollisionMgr&) = delete;
	CollisionMgr& operator=(const CollisionMgr&) = delete;

	
	static bool PointVsRect(const sf::Vector2f& l_p, ASprite& l_spr);
	static bool RectVsRect(ASprite& l_spr1, ASprite& l_spr2);
	static bool RayVsRect(const sf::Vector2f& l_ray_origin, const sf::Vector2f& l_ray_dir, rect& target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& t_hit_near);
	static bool DynamicRectVsRect(const ASprite& l_in,const ASprite& l_target, sf::Vector2f& cp, sf::Vector2f& cn, float& ct, const sf::Time& dt);
	static bool HandleCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>>& l_sprVec, const sf::Time& l_dt);
	




};

#endif