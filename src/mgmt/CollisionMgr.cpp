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
				collisionOccurred = true;

				z.push_back({ tile.get(), ct });
			}
			if (cn.y == -1.f)
			{
				player->SetGrounded(true);
				dispatch(player->fsm, evt_Landed{});
			}
			if (cn.y == 1.f)
			{
				dispatch(player->fsm, evt_ReachedJumpPeak{});
			}
		}

		
		std::sort(z.begin(), z.end(), [](const std::pair<Tile*, float>& a, const std::pair<Tile*, float>& b)
			{
				return a.second < b.second;
			});

		for (auto& j : z)
		{
			if (DynamicRectVsRect(*player, *j.first, cp, cn, ct , l_dt))
			{
				if (ct <= 1.f)
				{

				}
				player->vel().x += cn.x * abs(player->vel().x) * (1.f - ct);
				player->vel().y += cn.y * abs(player->vel().y) * (1.f - ct);
			}
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
	rect r;
	r.pos = l_spr.getBoxGlobalBounds().getPosition();
	r.size = l_spr.getBoxGlobalBounds().getSize();
	r.vel = { 0.f,0.f };

	return p.x >= r.pos.x && p.y >= r.pos.y && p.x < r.pos.x + r.size.x && p.y < r.pos.y + r.size.y;
}

bool CollisionMgr::RectVsRect(ASprite& l_spr1, ASprite& l_spr2)
{
	rect r1;
	r1.pos = l_spr1.getBoxGlobalBounds().getPosition();
	r1.size = l_spr1.getBoxGlobalBounds().getSize();
	r1.vel = { 0.f,0.f };
	rect r2;
	r2.pos = l_spr2.getBoxGlobalBounds().getPosition();
	r2.size = l_spr2.getBoxGlobalBounds().getSize();
	r2.vel = { 0.f,0.f };

	return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x &&
		r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);

}

bool CollisionMgr::RayVsRect(const sf::Vector2f& ray_origin, const sf::Vector2f& ray_dir, rect& target, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near)
{
	
	sf::Vector2f tl = target.pos;
	sf::Vector2f br = { tl.x + target.size.x, tl.y + target.size.y };


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

	return true;
}

bool CollisionMgr::DynamicRectVsRect(const ASprite& in, const ASprite& target, sf::Vector2f& cp, sf::Vector2f& cn, float& ct, const sf::Time& dt)
{


	if (in.getVel().x == 0 && in.getVel().y == 0)
	{
		return false;
	}


	rect expanded_target;

	expanded_target.pos = sf::Vector2f(target.getSpr().getPosition().x - target.getSpr().getOrigin().x - (float)in.getCurrBoxRect().width / 2.f, target.getSpr().getPosition().y - target.getSpr().getOrigin().y - (float)in.getCurrBoxRect().height / 2.f);
	expanded_target.size = sf::Vector2f((float)target.getSpr().getTextureRect().height + (float)in.getCurrBoxRect().width, (float)target.getSpr().getTextureRect().height + (float)in.getCurrBoxRect().height);
	expanded_target.vel = sf::Vector2f({ 0.f, 0.f });





	if (RayVsRect({ in.bLeft() + (float)in.getCurrBoxRect().width / 2.f, in.bTop() + (float)in.getCurrBoxRect().height / 2.f }, {in.getVel().x * dt.asSeconds(), in.getVel().y * dt.asSeconds()}, expanded_target, cp, cn, ct))
	{
		if (ct <= 1.0f)
		{
			return true;
		}	
	}

	return false;
}

float CollisionMgr::PlayerVTile(Player& l_player, Tile& l_tile, sf::Vector2f& contact_normal, const sf::Time& l_dt)
{
	




			float contact_time = SweptAABB(l_player, l_tile, contact_normal.x, contact_normal.y);
			if (contact_time < 0.f || contact_time >= 1.0f)
			{
				return 1.1f;

			}

			if (contact_normal.y == -1)
			{
				l_player.SetGrounded(true);
				l_player.SetJustJumped(false);
				dispatch(l_player.fsm, evt_Landed{});
			}

			if(contact_normal.y == -1.f || contact_normal.x == -1.f || contact_normal.x == 1.f || contact_normal.y == 1.f)
				return contact_time;

	return 1.1f;
}



float CollisionMgr::SweptAABB(ASprite& l_spr1, ASprite& l_spr2, float& normalx, float& normaly)
{
	struct r
	{
		float x, y, w, h, vx, vy;
	};

	r b1{ l_spr1.bLeft(),l_spr1.bTop(), (float)l_spr1.getCurrBoxRect().width,  (float)l_spr1.getCurrBoxRect().height, l_spr1.vel().x,l_spr1.vel().y };
	r b2{};
	if (dynamic_cast<Tile*>(&l_spr2))
	{
	   b2 = r{ l_spr2().getPosition().x - l_spr2().getOrigin().x, l_spr2().getOrigin().y, (float)l_spr2().getTextureRect().width,  (float) l_spr2().getTextureRect().height,0.f, 0.f};

	}
	else
	{
		b2 = r{ l_spr2.bLeft(),l_spr2.bTop(), (float)l_spr2.getCurrBoxRect().width,  (float)l_spr2.getCurrBoxRect().height, l_spr2.vel().x,l_spr2.vel().y };

	}

	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	// find the distance between the objects on the near and far sides for both x and y 
	if (b1.vx > 0.0f)
	{
		xInvEntry = b2.x - (b1.x + b1.w);
		xInvExit = (b2.x + b2.w) - b1.x;
	}
	else
	{
		xInvEntry = (b2.x + b2.w) - b1.x;
		xInvExit = b2.x - (b1.x + b1.w);
	}

	if (b1.vy > 0.0f)
	{
		yInvEntry = b2.y - (b1.y + b1.h);
		yInvExit = (b2.y + b2.h) - b1.y;
	}
	else
	{
		yInvEntry = (b2.y + b2.h) - b1.y;
		yInvExit = b2.y - (b1.y + b1.h);
	}

	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
	float xEntry, yEntry;
	float xExit, yExit;

	if (b1.vx == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / b1.vx;
		xExit = xInvExit / b1.vx;
	}

	if (b1.vy == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / b1.vy;
		yExit = yInvExit / b1.vy;
	}

	//if (yEntry > 1.0f) yEntry = -std::numeric_limits<float>::infinity();
	//if (xEntry > 1.0f) xEntry = -std::numeric_limits<float>::infinity();
	// Find the earliest/latest times of collision.
	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	if (entryTime > exitTime) return 1.0f; // This check was correct.
	if (xEntry < 0.0f && yEntry < 0.0f) return 1.0f;
	if (xEntry < 0.0f) {
		// Check that the bounding box started overlapped or not.
		if (b1.x + b1.w < b2.x || b1.x > b2.x + b2.w) return 1.0f;
	}
	if (yEntry < 0.0f) {
		// Check that the bounding box started overlapped or not.
		if (b1.y + b1.h < b2.y || b1.y > b2.y + b2.h) return 1.0f;
	}

	// if there was no collision
	/*if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
	{
		normalx = 0.0f;
		normaly = 0.0f;
		return 1.0f;
	}*/

	//else // if there was a collision 
	//{
		// calculate normal of collided surface
	if (xEntry > yEntry)
	{
		if (xInvEntry < 0.0f)
		{
			normalx = 1.0f;
			normaly = 0.0f;
		}
		else
		{
			normalx = -1.0f;
			normaly = 0.0f;
		}
	}
	else
	{
		if (yInvEntry < 0.0f)
		{
			normalx = 0.0f;
			normaly = 1.0f;
		}
		else
		{
			normalx = 0.0f;
			normaly = -1.0f;
		}
	} // return the time of collisionreturn entryTime; 
//}

	return entryTime;
}

void CollisionMgr::Bounce(ASprite& b1, float l_remTime, float& normalx, float& normaly)
{

	b1.vel().x *= l_remTime;
	b1.vel().y *= l_remTime;
	if (abs(normalx) > 0.0001f) b1.vel().x = -(b1.vel().x);
	if (abs(normaly) > 0.0001f) b1.vel().y = -(b1.vel().y);

}

void CollisionMgr::Slide(ASprite& box, float l_remTime, float& normalx, float& normaly)
{

	// slide 
	float dotprod = (box.vel().x * normaly + box.vel().y * normalx) * l_remTime;
	//SetVelocity(box, {dotprod * normaly, dotprod * normalx});
}

void CollisionMgr::Push(ASprite& box, float l_remTime, float& normalx, float& normaly, sf::Vector2f& l_tmpVel)
{

	// push 
	float magnitude = sqrt((box.vel().x * box.bLeft() + box.vel().y * box.vel().y)) * l_remTime;
	float dotprod = box.vel().x * normaly + box.vel().y * normalx;

	if (dotprod > 0.0f) dotprod = 1.0f;
	else if (dotprod < 0.0f) dotprod = -1.0f;

	box.vel() = {dotprod * normaly * magnitude, dotprod * normalx * magnitude};
	// AddToVelocity(box, { dotprod * normaly * magnitude , dotprod * normalx * magnitude });
}
