#include "CollisionMgr.hpp"
#include <sprite/sprites/Bullet.hpp>

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
sf::FloatRect CollisionMgr::m_currOverlap = { { 0.f, 0.f }, { 0.f,0.f } };
sf::FloatRect CollisionMgr::m_prevOverlap = { { 0.f, 0.f }, { 0.f,0.f } };


bool CollisionMgr::CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec, sf::Vector2f& cp, sf::Vector2f& cn, float& t, const sf::Time& l_dt)
{
	bool collisionOccurred = false;
	SpriteType playerCollidedType = SpriteType::Count;
	std::vector<std::pair<std::shared_ptr<ASprite>, float>> z;
	z.clear();
	// now check from lower bounds to upper bounds only as 
	// this is the space the sprite is in before and after the movement
	// assuming sprB is static, not moving


	for (auto& s : l_sprVec)
	{
		// if same , try again
		if (&l_sprA == s.get())
		{
			continue;
		}


		// for player to tiles
		if (l_sprA.getType() == SpriteType::Actor && (*s).getType() == SpriteType::Tile && dynamic_cast<Player*>(&l_sprA) != nullptr)
		{
			if (CheckCollisionAndResolve(l_sprA, *s, CollisionStrategy::Push, l_dt.asSeconds()))
			{
				collisionOccurred = true;

			}
		}

			//playerCollidedType = SpriteType::Tile;
			//if (DynamicRectVsRect2(l_sprA, *s, cp, cn, t, l_dt))
			//{
			//	collisionOccurredPlayerOnTile = true;
			//	z.push_back({ s, t });
			//	//l_sprA.vel().x = 0.f;
			//	//l_sprA.vel().y = 0.f;
			//	l_sprA.SetGrounded(true);	
			//}
		//	playerCollidedType = SpriteType::Tile;

		//	if (l_sprA.vel().y != 0.f)
		//	{
		//		auto tmp = l_sprA.vel().x;
		//		l_sprA.vel().x = 0.f;
		//		if (DynamicRectVsRect2(l_sprA, *s, cp, cn, t, l_dt) && fabs(t) <= 1.f )
		//		{
		//			if (cn.y != 0.f)
		//			{
		//				if (cn.y == -1.f)
		//				{

		//					l_sprA.vel().y = 0;
		//					if (l_sprA().getPosition().y + l_sprA().getOrigin().y >= cp.y)
		//					{
		//						//push up
		//						l_sprA.vel().y = 0.f;

		//						auto diff = (l_sprA().getPosition().y - l_sprA().getOrigin().y) - cp.y;
		//						l_sprA().move({ 0.f, (l_sprA.GetRect().height + diff) });
		//						l_sprA.SetGrounded(true);
		//						dispatch(dynamic_cast<Player*>(&l_sprA)->fsm, evt_Landed {});
		//					
		//					}
		//					else
		//					{
		//						// push down

		//						l_sprA.vel().y = 0.f;

		//						auto diff = (l_sprA().getPosition().y - l_sprA().getOrigin().y) - cp.y;
		//						l_sprA().move({ 0.f, (l_sprA.GetRect().height + diff)});
		//						l_sprA.SetGrounded(true);
		//						dispatch(dynamic_cast<Player*>(&l_sprA)->fsm, evt_Landed {});
		//					}
		//				}
		//			}
		//		}
		//		l_sprA.vel().x = tmp;

		//	}
		//	
		//	if (DynamicRectVsRect2(l_sprA, *s, cp, cn, t, l_dt))
		//	{
		//		auto tmp = sf::Vector2f{ std::abs(l_sprA.vel().x), std::abs(l_sprA.vel().y) };
		//		collisionOccurred = true;
		//		if (cn.x != 0.0f)
		//		{
		//			if (cn.x == 1.f)
		//			{
		//				// push left
		//				
		//				if (l_sprA().getPosition().x - l_sprA().getOrigin().x < cp.x)
		//				{
		//					//push up
		//					l_sprA.vel().x = 0.f;

		//					auto diff = cp.x - (l_sprA().getPosition().x + l_sprA().getOrigin().x);
		//					l_sprA().move({ (l_sprA.GetRect().width + diff), 0.f });
		//				}
		//				else
		//				{
		//					//push up
		//					l_sprA.vel().x = 0.f;

		//					auto diff = cp.x - (l_sprA().getPosition().x + l_sprA().getOrigin().x);
		//					l_sprA().move({ (l_sprA.GetRect().width + diff), 0.f });
		//				}
		//			}
		//			else
		//			{
		//				// push right
		//				if (l_sprA().getPosition().x + l_sprA().getOrigin().x >= cp.x)
		//				{
		//					//push left
		//					l_sprA.vel().x = 0.f;

		//					auto diff = (l_sprA().getPosition().x - l_sprA().getOrigin().x) - cp.x;
		//					l_sprA().move({-1.f*(l_sprA.GetRect().width + diff), 0.f });
		//				}
		//				else
		//				{

		//					//push up
		//					l_sprA.vel().x = 0.f;

		//					auto diff = (l_sprA().getPosition().x - l_sprA().getOrigin().x) - cp.x;
		//					l_sprA().move({-1.f*(l_sprA.GetRect().width + diff), 0.f 	}
		//			}
		//	/////////////////////////////l_sprA.vel().y += (cn.y * tmp.y * (1.f - t));
		//	///////////////////////////l_sprA.vel().x += (cn.x * tmp.x * (1.f - t));
		//		//////////////////////////////////////
		//		/*/////////////////if (cn.y == -1.f)
		//		//////////////////{
		//		//////////////////	l_sprA.SetGrounded(true);
		//		//////////////////	collisionOccurredPlayerOnTile;
		//		//////////////////}
		//		//////////////////z.push_back({ s, t });*/
		//		/////////////////////////
		//	}
		//}

		// for bullet to tiles
			if (l_sprA.getType() == SpriteType::Projectile && (*s).getType() == SpriteType::Tile)
			{
				if (RectVsRect(l_sprA, *s))
				{
					l_sprA.SetAlive(false);

					collisionOccurred = true;
				}

			}

			// for bullet to enemies
			if (l_sprA.getType() == SpriteType::Projectile && (*s).getType() == SpriteType::Enemy)
			{
				if (dynamic_cast<Projectile*>(&l_sprA)->GetFriendly())
				{
					if (RectVsRect(l_sprA, *s))
					{
						dynamic_cast<Enemy*>(s.get())->TakeHit(dynamic_cast<Projectile*>(&l_sprA)->GetDamage());
						l_sprA.SetAlive(false);
						collisionOccurred = true;
					}
				}

			}

			// for bullet to player
			if (l_sprA.getType() == SpriteType::Projectile && (*s).getType() == SpriteType::Actor && (*s).getName() == SpriteName::Player && dynamic_cast<Player*>(s.get()) != nullptr)
			{
				if (!dynamic_cast<Projectile*>(&l_sprA)->GetFriendly())
				{
					if (RectVsRect(l_sprA, *s))
					{
						dynamic_cast<Player*>(s.get())->TakeHit(dynamic_cast<Projectile*>(&l_sprA)->GetDamage());
						l_sprA.SetAlive(false);
						collisionOccurred = true;

					}
				}
			}

	}
		///////////////////
		/* ///////////////std::sort(z.begin(), z.end(), [](const std::pair<std::shared_ptr<ASprite>, float>& a, const std::pair<std::shared_ptr<ASprite>, float>& b)
		////////	{
		///////		return a.second < b.second;
		/////// 	});*/
		//////////////

		///////////////for (auto j : z)
		////////////////{
		////////////////		if (DynamicRectVsRect2(l_sprA, *j.first, cp, cn, t, l_dt))
		////////////////		{
		//////////////
		////////////////					auto tmp = sf::Vector2f{ std::abs(l_sprA.vel().x), std::abs(l_sprA.vel().y) };
		////////////////					l_sprA.vel().x += (cn.x * tmp.x * (1.f - t));
		////////////////					l_sprA.vel().y += (cn.y * tmp.y * (1.f - t));
		//////////////
		////////////////					/*if (cn.y != 0.f)
		////////////////					{
		////////////////						l_sprA().move(0.f, l_sprA.vel().y * l_dt.asSeconds());
		////////////////					}
		////////////////					if (cn.x != 0.f)
		////////////////					{
		////////////////						l_sprA().move(l_sprA.vel().x * l_dt.asSeconds(), 0.f);
		////////////////					}
		////////////////					if (l_sprA().getPosition().y + l_sprA().getOrigin().y > cp.y && cn.y == -1.f)
		////////////////					{
		////////////////						l_sprA().setPosition(l_sprA().getPosition().x, cp.y - l_sprA().getOrigin().y);
		////////////////						l_sprA.vel().y = 0.f;
		////////////////					}
		////////////////					if (l_sprA().getPosition().y - l_sprA().getOrigin().y < cp.y && cn.y == 1.f)
		////////////////					{
		////////////////						l_sprA().setPosition(l_sprA().getPosition().x, cp.y - l_sprA().getOrigin().y);
		////////////////						l_sprA.vel().y = 0.f;
		////////////////					}
		////////////////					if (l_sprA().getPosition().x + l_sprA().getOrigin().x > cp.x && cn.x == -1.f)
		////////////////					{
		////////////////						l_sprA().setPosition(cp.x - l_sprA().getOrigin().x, l_sprA().getPosition().y);
		////////////////						l_sprA.vel().x = 0.f;
		////////////////					}
		////////////////					if (l_sprA().getPosition().x - l_sprA().getOrigin().x < cp.x && cn.x == 1.f)
		////////////////					{
		////////////////						l_sprA().setPosition(cp.x + l_sprA().getOrigin().x, l_sprA().getPosition().y);
		////////////////						l_sprA.vel().x = 0.f;
		////////////////					}*/
		////////////////				
		////////////////			}
		///////////////    }

		return collisionOccurred;
}


bool CollisionMgr::IsColliding(ASprite& l_sprA, ASprite& l_sprB)
{
	auto tmp1 = l_sprA().getOrigin();
	auto tmp2 = l_sprB().getOrigin();

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


bool CollisionMgr::CheckCollisionAndResolve(ASprite& l_dynamicSpr, ASprite& l_staticSpr, CollisionStrategy l_collideStrat, float l_dt)
{
	Box box{};
	Box::SetupBox(box, l_dynamicSpr);
	Box block{};
	Box::SetupBox(block, l_staticSpr);

	std::unique_ptr<Box> broadphasebox = { std::move(Box::GetSweptBroadphaseBox(box, l_dt)) };

	if (broadphasebox != nullptr)
	{
		if (Box::BroadphaseCheck(*broadphasebox, block))
		{
			float normalx, normaly;
			float collisiontime = Box::SweptAABB(box, block, normalx, normaly);
			if (collisiontime > 0.f && collisiontime <= 1.f)
			{
				float remainingtime = 1.0f - collisiontime;

				if (normaly == 1.f)
				{
					
				

					if (dynamic_cast<Player*>(&l_dynamicSpr) != nullptr)
					{
						dispatch(dynamic_cast<Player*>(&l_dynamicSpr)->fsm, evt_ReachedJumpPeak {});
					}
				}
				if (normalx == -1.f || normalx == 1.f)
				{
					if (dynamic_cast<Player*>(&l_dynamicSpr) != nullptr)
					{
						dynamic_cast<Player*>(&l_dynamicSpr)->SetMoving(false,(normalx == -1.f) ? true : false);
					}
					int i = 0;
				}
				if (normaly == -1.f)
				{
					
					

					if (dynamic_cast<Player*>(&l_dynamicSpr) != nullptr)
					{
						dynamic_cast<Player*>(&l_dynamicSpr)->SetGrounded(true);
						dispatch(dynamic_cast<Player*>(&l_dynamicSpr)->fsm, evt_Landed {});
					}
				}
				
				//Box::UpdateBoxVelocity(box, {box.vx, box.vy}, remainingtime);


				// resolution Strategy
				switch (l_collideStrat)
				{
				case CollisionStrategy::Bounce:
				{
					Box::Bounce(box, remainingtime, normalx, normaly);
				}
				break;
				case CollisionStrategy::Slide:
				{
					Box::Slide(box, remainingtime, normalx, normaly);
				}
				break;
				case CollisionStrategy::Push:
				{
					Box::Push(box, remainingtime, normalx, normaly);
				}
				break;
				default:
					break;
				}

				//Box::MoveBox(box, {box.vx, box.vy}, 1.f);
			
				Box::SetSpriteVelocity(box, { box.vx, box.vy });

				return true;
			}


		}
		
	}
	return false;
}
