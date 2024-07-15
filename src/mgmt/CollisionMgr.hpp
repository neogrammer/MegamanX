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

	static void CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec, const sf::Time& l_dt);
	static bool IsColliding(ASprite& l_sprA, ASprite& l_sprB);
	static void ResolveActor(ASprite& l_spr);
	static void ResolveProjectile(ASprite& l_sprA, ASprite& l_sprB);

	static bool PointVsRect(const sf::Vector2f& l_p, ASprite& l_spr);
	static bool RectVsRect(ASprite& l_spr1, ASprite& l_spr2);
	
	// get ray_origin as the players center point, and the dir from the position to the pos +  vel * l_dt (aka, where it should be if no collisions), to pass in function
	// make vf2d cp, cn;  and float t; outside function and pass it in
	static bool RayVsRect(const sf::Vector2f& l_ray_origin, const sf::Vector2f& l_ray_dir, rect& target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& t_hit_near);
	// if (RayVsRect(...) && t <= 1.f) // collision! vals below are meaningful now
	// now you have cp, cn, and t you can use for dynamic rectvrect collision resolution
	// point_Where_Player_Should_Land_After_Collision = cp.x - ((player.width / 2.f) * cn.x),  cp.y - ((player.height / 2.f) * cn.y)
	
	// #include <algorithm>
	// #include <vector>
	// make std::vector<rect> vRects; // and push back any targets into it
	// but make vRects[0] the sprA Rect testing among the others within the range from where the player is to where the player is going  truncated in the top leftto get the tiles to test
	// in the algorithm
	static bool DynamicRectVsRect(ASprite& l_in, ASprite& l_target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& l_contact_time, const sf::Time& fElapsedTime);
	// 
	//  std::vector<std::pair<int, float>> z;
	//    
	//  for (int i = 1; i < vRects.size(); i++)
	// {
	// 
	// 
	//   if (DynamicRectVsRect(l_sprA,(ASprite*) *s, cp, cn, ct, l_dt))
	//   {
	//		z.push_back({ s, ct });
	//	 }
	// 
	//	}
	// 
	//  std::sort(z.begin(), z.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
	// {
	//		return a.second < b.second;
	// });
	// 
	//  //vRects[0].vel = player().vel();
	//  for (auto j : z)
	//  {
	//		if (DynamicRectVsRect(l_sprA, *vRects[j.first], cp, cn, ct, fElapsedTime))
	//		{
	// 
	// 
	//		
	// 
	//		auto tmp = vf2d{std::abs(vRects[0]->vel().x), std::abs(vRects[0]->vel().y)};
	//      vRects[0]->vel().x += cn.x * tmp.x * (1.f - ct);
	//		 vRects[0]->vel().y += cn.y * tmp.y * (1.f - ct);
	//		
	//		
	//			
	//		}
	//   }
	// 
	//		player()vel().x = vRects[0].vel.x;
	// 		player()vel().y = vRects[0].vel.y;
	// 
	// 
	//    then...  player().move(player()->vel() * l_dt);
	//   } end of update method	
	
private:
	static sf::FloatRect m_currOverlap;
	static sf::FloatRect m_prevOverlap;

};

#endif