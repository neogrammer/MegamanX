#include "CollisionMgr.hpp"
#include <sprite/sprites/Bullet.hpp>
#include <sprite/sprites/BuzzSaw.hpp>
#include <sprite/sprites/Player.hpp>
#include <sprite/sprites/Tile.hpp>
#include <sprite/sprites/BuzzBird.hpp>


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <misc/globals.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <FSM/duck_fold.hpp>


bool CollisionMgr::HandleCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>>& l_sprVec, const sf::Time& l_dt)
{
	if (l_sprVec.empty()) return false; // nothing to test against
	std::vector<std::shared_ptr<Tile>> tiles;
	tiles.clear();
	tiles.reserve(l_sprVec.size());
	std::vector< std::shared_ptr<Projectile>> projectiles;
	projectiles.clear();
	projectiles.reserve(l_sprVec.size());
	std::vector< std::shared_ptr<Enemy>> enemies;
	enemies.clear();
	enemies.reserve(l_sprVec.size());
	sf::Vector2f contact_normal{};
	float contact_time{};
	for (auto& s : l_sprVec)
	{
		switch (s->getType())
		{
		case SpriteType::Tile:
			tiles.push_back(std::dynamic_pointer_cast<Tile>(s));
			break;
		case SpriteType::Enemy:
		{
				if (s->getName() == SpriteName::BuzzBird)
					enemies.push_back(std::dynamic_pointer_cast<BuzzBird>(s));
		}
		break;
		case SpriteType::Projectile:
		{
			//if (s->getName() == SpriteName::BuzzSaw)
				//projectiles.push_back(std::make_shared<BuzzSaw>(std::dynamic_pointer_cast<std::shared_ptr<BuzzSaw>>(s)));
			if (s->getName() == SpriteName::BusterBullet)
				projectiles.push_back(std::dynamic_pointer_cast<Bullet>(s));
		}
			break;
		default:
			break;
		}
	}
	tiles.shrink_to_fit();
	enemies.shrink_to_fit();
	projectiles.shrink_to_fit();
	Player* player = nullptr;
	
	if (l_sprA.getType() == SpriteType::Actor)
		player = dynamic_cast<Player*>(&l_sprA);


	bool collisionOccurred{ false };


	// check collisions with the map tiles
	
	if (!tiles.empty() && player)
	{
		sf::Vector2f cp, cn;
		float ct = 0.f;

		std::vector<std::pair<Tile*, float>> z; 	// for sorting collisions for proper swept aabb resolution
		z.clear();
		
		for (auto& tile : tiles)
		{
			
			// first check if a collision occurred and push it back, dont resolve yet
			if (DynamicRectVsRect(*player, *tile,cp, cn, ct, l_dt))
			{

				z.push_back({ tile.get(), ct });

				//ct = 0.f;
			}
			/*if (cn.y == -1.f)
			{
				player->SetGrounded(true);
				dispatch(player->fsm, evt_Landed{});
			}
			if (cn.y == 1.f)
			{
				dispatch(player->fsm, evt_ReachedJumpPeak{});
			}*/
		}

		
		std::sort(z.begin(), z.end(), [](const std::pair<Tile*, float>& a, const std::pair<Tile*, float>& b)
			{
				return a.second < b.second;
			});

		//for (auto it = z.begin(); it != z.end(); it++)///&j : z)
		for (auto& j : z)
		{
			
			if (DynamicRectVsRect(*player, *j.first, cp, cn, ct, l_dt) && ct <= 1.0)
			{
				//if (ct == j.second)
				//{
				//	//player->vel().x += cn.x * abs(player->vel().x) * (1.f - ct);
				//	//player->vel().y += cn.y * abs(player->vel().y) * (1.f - ct);
				//}
				//else
				//{
				//	if (ct > j.second)
				//	{


				//		//player->vel().x += -cn.x * abs(player->vel().x) * (1.f - ct); 
				//		//player->vel().y += -cn.y * abs(player->vel().y) * (1.f - ct); 
				//	}
				//	else
				//	{
				if (abs(player->vel().x) - abs(cn.x * abs(player->vel().x) * l_dt.asSeconds() * (1 - ct))
				>= abs(player->vel().x))
				{
					int i = 0;
					player->vel().x += cn.x * abs(player->vel().x) * (1 - j.second);
					player->vel().y += cn.y * abs(player->vel().y) * (1 - j.second);

				}
				else
				{
					player->vel().x += cn.x * abs(player->vel().x) * l_dt.asSeconds() * (1 - ct);
					player->vel().y += cn.y * abs(player->vel().y) * l_dt.asSeconds() * (1 - ct);
				}
					//}
				//}
				//player->move(player->vel(), l_dt.asSeconds());


				collisionOccurred = true;
				if (cn.y == -1)
				{
					player->SetGrounded(true);
					dispatch(player->fsm, evt_Landed{});

				}
				
			}
		}

		if (!collisionOccurred)
		{
			dispatch(player->fsm, evt_Fell {});
		}
		else
		{
			
			player->SetGrounded(true);
			dispatch(player->fsm, evt_Landed {});
			

		}
		
	}

	if (!tiles.empty() && !projectiles.empty())
	{
		for (auto& bullet : projectiles)
		{
			for (auto& tile : tiles)
			{
				if (RectVsRect(*(ASprite*)bullet.get(), *(ASprite*)tile.get()))
				{
					(*(ASprite*)bullet.get()).SetAlive(false);

					collisionOccurred = true;
				}
			}
		}
	}

	if (!enemies.empty() && !projectiles.empty())
	{
		for (auto& bullet : projectiles)
		{
			for (auto& enemy : enemies)
			{
				if ((*bullet).GetFriendly())
				{
					if (RectVsRect(*(ASprite*)bullet.get(), *(ASprite*)enemy.get()))
					{
						(*(ASprite*)bullet.get()).SetAlive(false);
						enemy->TakeHit(bullet->GetDamage());
						collisionOccurred = true;
					}
				}
			}
		}
	}
	
	if (player && !projectiles.empty())
	{
		for (auto& bullet : projectiles)
		{
				if (!(*bullet).GetFriendly())
				{
					if (RectVsRect(*(ASprite*)bullet.get(), *(ASprite*)player))
					{
						(*(ASprite*)bullet.get()).SetAlive(false);
						player->TakeHit(bullet->GetDamage());
						collisionOccurred = true;
					}
				}
		}
	}
		

	return collisionOccurred;

}



bool CollisionMgr::PointVsRect(const sf::Vector2f& l_p, ASprite& l_spr)
{
	sf::Vector2f p{ l_p.x, l_p.y };
	rect r;
	r.pos = l_spr.boxTL();
	r.size = { l_spr.boxBR() - l_spr.boxTL() };
	r.vel = { 0.f,0.f };

	return p.x >= r.pos.x && p.y >= r.pos.y && p.x < r.pos.x + r.size.x && p.y < r.pos.y + r.size.y;
}

bool CollisionMgr::RectVsRect(ASprite& l_spr1, ASprite& l_spr2)
{
	rect r1;
	r1.pos = { l_spr1.boxTL() };
	r1.size = { l_spr1.boxBR() - l_spr1.boxTL() };
	r1.vel = { l_spr1.vel() };
	rect r2;
	r2.pos = { l_spr2.boxTL() };
	r2.size = { l_spr2.boxBR() - l_spr2.boxTL() };
	r2.vel = { l_spr2.vel() };

	return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x &&
		r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);

}

bool CollisionMgr::RayVsRect(const sf::Vector2f& ray_origin, const sf::Vector2f& ray_dir, rect& target, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near)
{
	
	/*sf::Vector2f tl = target.pos;
	sf::Vector2f br = { tl.x + target.size.x, tl.y + target.size.y };*/

	sf::Vector2f t_near = { (target.pos.x - ray_origin.x) / ray_dir.x ,  (target.pos.y - ray_origin.y) / ray_dir.y };
	sf::Vector2f t_far = { (target.pos.x + target.size.x - ray_origin.x) / ray_dir.x ,  (target.pos.y + target.size.y - ray_origin.y) / ray_dir.y };


	/*float near_t_x = (tl.x - ray_origin.x) / ray_dir.x;
	float far_t_x = (br.x - ray_origin.x) / ray_dir.x;

	float near_t_y = (tl.y - ray_origin.y) / ray_dir.y;
	float far_t_y = (br.y - ray_origin.y) / ray_dir.y;*/
	
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);


	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;
	
	// this put into the function P(t) = P + Dt gives the point of ray intersection of the rectangle
	t_hit_near = std::max(t_near.x, t_near.y);
	float t_hit_far = std::min(t_far.x, t_far.y);

	if (t_hit_far < 0) return false;

	contact_point.x = ray_origin.x + t_hit_near * ray_dir.x;
	contact_point.y = ray_origin.y + t_hit_near * ray_dir.y;


	if (t_near.x > t_near.y)
	{
		if (ray_dir.x < 0.f)
			contact_normal = { 1.f,0.f };
		else
			contact_normal = { -1.f,0.f };
	}
	else if (t_near.x < t_near.y)
	{
		if (ray_dir.y < 0.f)
			contact_normal = { 0.f,1.f };
		else
			contact_normal = { 0.f,-1.f };
	}

	return true;

	//if (std::isnan(far_t_x) || std::isnan(far_t_y)) return false;
	//if (std::isnan(near_t_x) || std::isnan(near_t_y)) return false;
}

bool CollisionMgr::DynamicRectVsRect(const ASprite& in, const ASprite& target, sf::Vector2f& cp, sf::Vector2f& cn, float& ct, const sf::Time& dt)
{


	if (in.getVel().x == 0 && in.getVel().y == 0)
	{
		return false;
	}


	rect expanded_target;

	expanded_target.pos = sf::Vector2f{ target.getSpr().getPosition().x - target.getSpr().getOrigin().x - ((float)in.getSpr().getTextureRect().width / 2.f), target.getSpr().getPosition().y - target.getSpr().getOrigin().y - ((float)in.getSpr().getTextureRect().height / 2.f)};
	expanded_target.size = sf::Vector2f{ (float)target.getSpr().getTextureRect().width + (float)in.getSpr().getTextureRect().width, (float)target.getSpr().getTextureRect().height + (float)in.getSpr().getTextureRect().height };
	expanded_target.vel = { 0.f,0.f };




	/*cp = { 0.f,0.f };
	cn = { 0.f,0.f };
	ct = 0.f;*/

	//if (RayVsRect(  in.getSpr().getPosition() , {in.getVel().x, in.getVel().y}, expanded_target, cp, cn, ct) && ct <= 1.0f)
	//{

		if (RayVsRect(in.getSpr().getPosition(), { in.getVel().x * dt.asSeconds(), in.getVel().y * dt.asSeconds() }, expanded_target, cp, cn, ct))
		{
			if (ct <= 1.0f)
			{
				/*auto tmp = sf::Vector2f{ abs(in.getVel().x) * dt.asSeconds(),abs(in.getVel().x) * dt.asSeconds() };
				cp.x += ((float)in.getSpr().getTextureRect().width / 2.f) *  -cn.x - tmp.x * (1.f - ct);
				cp.y += ((float)in.getSpr().getTextureRect().height / 2.f) * -cn.y - tmp.y * (1.f - ct);*/
				
				return true;
			}
		}
	//}

	return false;
}
