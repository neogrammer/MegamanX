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
#include <misc/Box.hpp>


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

		std::vector<std::pair<Tile*, float>> z; 	// for sorting collisions for proper swept aabb resolution
		z.clear();
		for (auto& tile : tiles)
		{
			float t;
			sf::Vector2f contact_point;
			// first check if a collision occurred and push it back, dont resolve yet
			if (IsColliding(*player, *tile))
			{
				player->vel().y = 0;
				collisionOccurred = true;
				break;
			}
		}
				
			/*	, contact_normal, l_dt);
			if (contact_time < 1.0f && contact_time >= 0.f)
			{ 
				z.push_back({ tile.get(), t}); 
			}
		}
		std::sort(z.begin(), z.end(), [](const std::pair<Tile*, float>& a, const std::pair<Tile*, float>& b)
			{
				return a.second < b.second;
			});
		for (auto& j : z)
		{
			sf::Vector2f cp;
		
			if (CheckCollisionAndResolve(*player, *j.first, CollisionStrategy::Push, l_dt.asSeconds(), cp, contact_normal, j.second))
			{
				collisionOccurred = true;

				if (contact_normal.y == -1)
				{
					player->SetGrounded(true);
					dispatch(player->fsm, evt_Landed{});

				}
			}
		}*/
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


bool CollisionMgr::CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec, sf::Vector2f& cp, sf::Vector2f& cn, float& t, const sf::Time& l_dt)
{
	return false;
}

bool CollisionMgr::IsColliding(ASprite& l_sprA, ASprite& l_sprB)
{
	auto tmp1 = l_sprA().getOrigin();
	auto tmp2 = l_sprB().getOrigin();
	sf::FloatRect m_currOverlap{ { 0.f,0.0f }, { 0.f,0.f } }, m_prevOverlap{ {0.f,0.f} , { 0.f,0.f } };
	if (l_sprA().getGlobalBounds().intersects(l_sprB().getGlobalBounds(), m_currOverlap))
	{
		sf::FloatRect tmp = l_sprA().getGlobalBounds();
		tmp.left -= (l_sprA.vel().x * g::FPS60.asSeconds());
		tmp.top -= (l_sprA.vel().y * g::FPS60.asSeconds());
		tmp.intersects(l_sprB().getGlobalBounds(), m_prevOverlap);
		l_sprA().setOrigin(tmp1);
		l_sprB().setOrigin(tmp2);

		return true;
	}
	l_sprA().setOrigin(tmp1);
	l_sprB().setOrigin(tmp2);
	return false;
}



void CollisionMgr::ResolveActor(ASprite& l_spr)
{

	sf::FloatRect m_currOverlap{ { 0.f,0.0f }, { 0.f,0.f } }, m_prevOverlap{ {0.f,0.f} , { 0.f,0.f } };

	if (m_prevOverlap.width <= 0 && m_prevOverlap.height <= 0)
	{
		if (m_currOverlap.width >= m_currOverlap.height)
		{
			l_spr.vel().y = 0.f;
			l_spr().setPosition(l_spr().getPosition().x, l_spr().getPosition().y - m_currOverlap.height);
			l_spr.SetGrounded(true);
		}
		else
		{
			l_spr.vel().x = 0.f;
			l_spr().setPosition(l_spr().getPosition().x - m_currOverlap.width, l_spr().getPosition().y);
		}
	}

	/*
	else
	{
		if (abs(m_currOverlap.height - m_prevOverlap.height) <= abs(m_currOverlap.width - m_prevOverlap.width))
		{
			l_spr.vel().y = 0.f;
			l_spr().setPosition(l_spr().getPosition().x, l_spr().getPosition().y - (m_currOverlap.height - m_prevOverlap.height));
			l_spr.SetGrounded(true);
		}
		else
		{
			l_spr.vel().x = 0.f;
			l_spr().setPosition(l_spr().getPosition().x - m_currOverlap.width, l_spr().getPosition().y);
		}
	}*/
	//else
	//{
	/*	if (m_prevOverlap.width <= 0 && m_prevOverlap.height > 0)
		{
			l_spr.vel().x = 0.f;
			l_spr().setPosition(l_spr().getPosition().x - m_currOverlap.width, l_spr().getPosition().y);
		}
		else if (m_prevOverlap.height <= 0 && m_prevOverlap.width > 0)
		{
			l_spr.vel().y = 0.f;
			l_spr().setPosition(l_spr().getPosition().x, l_spr().getPosition().y - m_currOverlap.height);
			l_spr.SetGrounded(true);
		}*/
		
	//}
}

void CollisionMgr::ResolveProjectile(ASprite& l_sprA, ASprite& l_sprB)
{
	sf::FloatRect m_currOverlap{ { 0.f,0.0f }, { 0.f,0.f } }, m_prevOverlap{ {0.f,0.f} , { 0.f,0.f } };

	if (m_prevOverlap.width < 0 && m_prevOverlap.height < 0)
	{
		l_sprA.vel().x = 0.f;
		l_sprA().setPosition(l_sprA().getPosition().x - m_currOverlap.width, l_sprA().getPosition().y);
	}
	else if (m_prevOverlap.width < 0)
	{
		l_sprA.vel().x = 0.f;
		l_sprA().setPosition(l_sprA().getPosition().x - m_currOverlap.width, l_sprA().getPosition().y);
	}
	else
	{
		l_sprA.vel().y = 0.f;
		l_sprA().setPosition(l_sprA().getPosition().x, l_sprA().getPosition().y - m_currOverlap.height);
	}
	

	switch (l_sprB.getType())
	{
	case SpriteType::Tile:
		l_sprA.SetAlive(false);
		break;
	case SpriteType::Actor:
		l_sprA.SetAlive(false);
		// do damage to actor
		// l_sprB.takeDamage(l_sprA.getDamage());
		break;
	default:
		break;

	}
}

bool CollisionMgr::PointVsRect(const sf::Vector2f& l_p, ASprite& l_spr)
{
	vf2d p{ l_p.x, l_p.y };
	rect r(&l_spr);
	r.Reset();

	return p.x >= r.pos.x && p.y >= r.pos.y && p.x < r.pos.x + r.size.x && p.y < r.pos.y + r.size.y;
}

bool CollisionMgr::RectVsRect(ASprite& l_spr1, ASprite& l_spr2)
{
	rect r1(&l_spr1);
	rect r2(&l_spr2);

	r1.Reset();
	r2.Reset();

	return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x &&
		r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);

}

bool CollisionMgr::RayVsRect(const sf::Vector2f& l_ray_origin, const sf::Vector2f& l_ray_dir, rect& target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& l_t_hit_near)
{
	vf2d ray_origin(l_ray_origin.x, l_ray_origin.y);
	vf2d ray_dir(l_ray_dir.x, l_ray_dir.y);



	vf2d contact_point;
	vf2d contact_normal;
	float& t_hit_near = l_t_hit_near;

	vf2d tl = target.pos;
	vf2d br = { tl.x + target.size.x, tl.y + target.size.y };


	float near_t_x = (tl.x - ray_origin.x) / ray_dir.x;
	float far_t_x = (br.x - ray_origin.x) / ray_dir.x;

	float near_t_y = (tl.y - ray_origin.y) / ray_dir.y;
	float far_t_y = (br.y - ray_origin.y) / ray_dir.y;
	
	if (std::isnan(far_t_x) || std::isnan(far_t_y)) return false;
	if (std::isnan(near_t_x) || std::isnan(near_t_y)) return false;


	if (near_t_x > far_t_x)
	{
		auto tmp = near_t_x;
		near_t_x = far_t_x;
		far_t_x = tmp;
	}

	if (near_t_y > far_t_y)
	{
		auto tmp = near_t_y;
		near_t_y = far_t_y;
		far_t_y = tmp;
	}
	if (near_t_x > far_t_y || near_t_y > far_t_x) return false;


	// this put into the function P(t) = P + Dt gives the point of ray intersection of the rectangle
	t_hit_near = std::max(near_t_x, near_t_y);
	float t_hit_far = std::min(far_t_x, far_t_y);

	if (t_hit_far < 0) return false;

	contact_point.x = ray_origin.x + t_hit_near * ray_dir.x;
	contact_point.y = ray_origin.y + t_hit_near * ray_dir.y;

	if (near_t_x > near_t_y)
	{
		if (ray_dir.x < 0.f)
			contact_normal = { 1.f,0.f };
		else
			contact_normal = { -1.f,0.f };
	}
	else if (near_t_x < near_t_y)
	{
		if (ray_dir.y < 0.f)
			contact_normal = { 0.f,1.f };
		else
			contact_normal = { 0.f,-1.f };
	}

	l_contact_point = { contact_point.x, contact_point.y };
	l_contact_normal = { contact_normal.x, contact_normal.y };
	// t_hit_near is a ref
	return true;
}

bool CollisionMgr::DynamicRectVsRect(ASprite& l_in, ASprite& l_target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& l_contact_time, const sf::Time& fElapsedTime)
{
	rect in(&l_in);
	in.Reset();
	rect target(&l_target);
	target.Reset();

	if (in.vel.x == 0 && in.vel.y == 0)
	{
		return false;
	}
	ASprite* spr = new Tile{};
	*spr = l_target;
	spr->vel() = l_target.vel();
	(*spr)().setPosition({ target.pos.x - (in.size.x / 2.f), target.pos.y - (in.size.y / 2.f) });
	(*spr)() = l_target();
	(*spr)().setTexture(*const_cast<sf::Texture*>(l_target().getTexture()));
	(*spr)().setTextureRect(l_target().getTextureRect());
	rect expanded_target(spr);
	expanded_target.Reset();
	expanded_target.size.x = target.size.x + in.size.x;
	expanded_target.size.y = target.size.y + in.size.y;
	(*spr)().setOrigin({ expanded_target.size.x / 2.f,expanded_target.size.y / 2.f });

	
	if (l_in.vel().x < 0)
	{

	}
	float offx{ 0.f }, offy{ 0.f };
	if (l_in.vel().x > 0)
	{
		offx = l_in().getOrigin().x;
	}
	else if (l_in.vel().x < 0)
	{
		offx = -1.f * l_in().getOrigin().x;
	}
	if (l_in.vel().y > 0)
	{
		offy = l_in().getOrigin().y;
	}
	else if (l_in.vel().y < 0)
	{
		offy = -1.f * l_in().getOrigin().y;
	}
	
	sf::Vector2f end_point = { l_in().getPosition().x + offx + (l_in.vel().x * fElapsedTime.asSeconds()), l_in().getPosition().y + offy + (l_in.vel().y * fElapsedTime.asSeconds())};
	sf::Vector2f dir = { end_point.x - l_in().getPosition().x, end_point.y - l_in().getPosition().y };
	//dir.x *= l_in().getOrigin().x;
	//dir.y *= l_in().getOrigin().y;

	if (RayVsRect(l_in().getPosition(), dir, expanded_target, l_contact_point, l_contact_normal, l_contact_time))
	{
		if (l_contact_time <= 1.0f)
		{
			delete spr;
			return true;
		}	
	}
	delete spr;
	return false;
}

bool CollisionMgr::DynamicRectVsRect2(ASprite& l_in, ASprite& l_target, sf::Vector2f& l_contact_point, sf::Vector2f& l_contact_normal, float& l_contact_time, const sf::Time& fElapsedTime)
{
	rect in(&l_in);
	in.Reset();
	rect target(&l_target);
	target.Reset();

	if (in.vel.x == 0 && in.vel.y == 0)
	{
		return false;
	}
	sf::Vector2f velDir = l_in.vel();
	float lengthOfVel = sqrtf((float)pow(l_in.vel().x, 2) + (float)pow(l_in.vel().y, 2));
	velDir.x /= lengthOfVel;
	velDir.y /= lengthOfVel;

	sf::Vector2f startPoint{ l_in().getPosition().x + (velDir.x * l_in().getOrigin().x),  l_in().getPosition().y + (velDir.y * l_in().getOrigin().y)};
	sf::Vector2f end_point = { startPoint.x + (l_in.vel().x * fElapsedTime.asSeconds()), startPoint.y + (l_in.vel().y * fElapsedTime.asSeconds())};
	sf::Vector2f dir = { end_point.x - startPoint.x, end_point.y - startPoint.y};

	if (RayVsRect(startPoint, dir, target, l_contact_point, l_contact_normal, l_contact_time))
	{
		if (l_contact_time <= 1.0f)
		{
			return true;
		}
	}
	return false;
}


bool CollisionMgr::CheckCollisionAndResolve(ASprite& l_dynamicSpr, ASprite& l_staticSpr, CollisionStrategy l_collideStrat, float l_dt, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& contact_time)
{
		
	
	
	

	
	

	//if (l_dynamicSpr.JustJumped() == true)
	//{
		//box.vy = Player::JumpForce;
		//l_dynamicSpr.vel().y = Player::JumpForce;
		//l_dynamicSpr.SetJustJumped(false);
	//}


	Box box{};
	Box::SetupBox(box, l_dynamicSpr);
	Box block{};
	Box::SetupBox(block, l_staticSpr);
	std::unique_ptr<Box> broadphasebox = { std::move(Box::GetSweptBroadphaseBox(box, l_dt)) };

	if (broadphasebox != nullptr)
	{
		if (Box::BroadphaseCheck(*broadphasebox, block))
		{


			

	
			contact_time = Box::SweptAABB(box, block, contact_normal.x, contact_normal.y);
			if (contact_time <= 0.f || contact_time > 1.0f)
			{
				return false;
				
			}
			
			if (contact_normal.y == -1)
			{
				l_dynamicSpr.SetGrounded(true);
				l_dynamicSpr.SetJustJumped(false);
				//l_dynamicSpr.vel().y = 0;
				//box.vy = 0.f;
				dispatch(dynamic_cast<Player*>(&l_dynamicSpr)->fsm, evt_Landed {});
			}
			
			
			float remainingtime = 1.0f - contact_time;
			

				
				
			
			switch (l_collideStrat)
			{
			case CollisionStrategy::Bounce:
			{
				Box::Bounce(box, remainingtime, contact_normal.x, contact_normal.y);
			}
			break;
			case CollisionStrategy::Slide:
			{
				Box::Slide(box, remainingtime, contact_normal.x, contact_normal.y);
			}
			break;
			case CollisionStrategy::Push:
			{
				Box::Push(box, remainingtime, contact_normal.x, contact_normal.y);
			}
			break;
			default:
				break;
			}
			
			return true;
		}
		
	}
	return false;
}

float CollisionMgr::PlayerVTile(Player& l_player, Tile& l_tile, sf::Vector2f& contact_normal, const sf::Time& l_dt)
{
	




			float contact_time = Box::SweptAABB(*l_player.boxMap[AnimType::None].at(0), *l_tile.boxMap[AnimType::None].at(0), contact_normal.x, contact_normal.y);
			if (contact_time < 0.f || contact_time >= 1.0f)
			{
				return 1.0f;

			}

			if (contact_normal.y == -1)
			{
				l_player.SetGrounded(true);
				l_player.SetJustJumped(false);
				//l_dynamicSpr.vel().y = 0;
				//box.vy = 0.f;
				dispatch(l_player.fsm, evt_Landed{});

			}

			if(contact_normal.y == -1.f || contact_normal.x == -1.f || contact_normal.x == 1.f || contact_normal.y == 1.f)
				return contact_time;

	return 1.0f;
}
