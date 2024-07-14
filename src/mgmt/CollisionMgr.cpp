#include "CollisionMgr.hpp"
#include <sprite/sprites/Bullet.hpp>

#include <sprite/sprites/Player.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <misc/globals.hpp>
sf::FloatRect CollisionMgr::m_currOverlap = { { 0.f, 0.f }, { 0.f,0.f } };
sf::FloatRect CollisionMgr::m_prevOverlap = { { 0.f, 0.f }, { 0.f,0.f } };


void CollisionMgr::CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec)
{
	auto tmp = l_sprA().getGlobalBounds();
	tmp.left -= tmp.width / 2.f;
	tmp.top -= tmp.height / 2.f;
	
	
	

	// now check from lower bounds to upper bounds only as 
	// this is the space the sprite is in before and after the movement
	// assuming sprB is static, not moving
	for (auto& s : l_sprVec)
	{
		if (&l_sprA == s.get())
		{
			continue;
		}
		
		auto tmp2 = (*s)().getGlobalBounds();
		tmp2.left -= tmp2.width / 2.f;
		tmp2.top -= tmp2.height / 2.f;
		
		
		if (IsColliding(l_sprA, *s))
		{
			switch (l_sprA.getType())
			{
			case SpriteType::Actor:
				ResolveActor(l_sprA);
				break;
			case SpriteType::Projectile:
				ResolveProjectile(l_sprA, *s);
				break;
			default:
				break;
			}

			break;
		}
	}
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

bool CollisionMgr::PointVsRect(const vf2d& p, rect& r)
{
	r.Reset();

	return p.x >= r.pos.x && p.y >= r.pos.y && p.x < r.pos.x + r.size.x && p.y < r.pos.y + r.size.y;
}

bool CollisionMgr::RectVsRect(rect& r1, rect& r2)
{
	r1.Reset();
	r2.Reset();
	return (r1.pos.x < r2.pos.x + r2.size.x && r1.pos.x + r1.size.x > r2.pos.x &&
		r1.pos.y < r2.pos.y + r2.size.y && r1.pos.y + r1.size.y > r2.pos.y);

}

bool CollisionMgr::RayVsRect(const vf2d& ray_origin, const vf2d& ray_dir, rect& target, vf2d& contact_point, vf2d& contact_normal, float& t_hit_near)
{
	target.Reset();

	double max = std::numeric_limits<double>::max();
	double inf = std::numeric_limits<double>::infinity();

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


	return true;
}

bool CollisionMgr::DynamicRectVsRect(rect& in, rect& target, vf2d& contact_point, vf2d& contact_normal, float& contact_time, const sf::Time& fElapsedTime)
{
	in.Reset();
	target.Reset();

	if (in.vel.x == 0 && in.vel.y == 0)
	{
		return false;
	}
	auto spr = *decltype(&target.spr){};
	spr->vel() = target.spr->vel();
	(*spr)().setPosition((*target.spr)().getPosition());
	(*spr)() = (*target.spr)();
	(*spr)().setTexture(*const_cast<sf::Texture*>(&(*target.spr)().getTexture()));
	(*spr)().setTextureRect((*target.spr)().getTextureRect());
	(*spr)().setOrigin((*target.spr)().getOrigin());

	//(*spr)().setTextureRect((*target.spr)().getGlobalBounds());

	rect expanded_target(spr);
	expanded_target.Reset();
	expanded_target.pos.x = target.pos.x - (in.size.x / 2.f);
	expanded_target.pos.y = target.pos.y - (in.size.y / 2.f);
	expanded_target.size.x = target.size.x + in.size.x;
	expanded_target.size.y = target.size.y + in.size.y;

	if (RayVsRect({ in.pos.x + in.size.x / 2.f, in.pos.y + in.size.y / 2.f }, { in.vel.x * fElapsedTime.asSeconds(), in.vel.y * fElapsedTime.asSeconds() }, expanded_target, contact_point, contact_normal, contact_time))
	{
		if (contact_time <= 1.0f)
		{
			delete spr;
			return true;
		}	
	}
	delete spr;
	return false;
}

	/*if (near_t_x < far_t_y && near_t_y < far_t_y)
	{
		vf2d normal{};
		if (near_t_y < near_t_x && ray_dir.x > 0.f && ray_dir.y > 0.f)
		{
			normal.x = -1.f;
			normal.y = 0.f;
		}
		else if (near_t_y > near_t_x && ray_dir.x > 0.f && ray_dir.y > 0.f)
		{
			normal.x = 0.f;
			normal.y = -1.f;
		}
		else if (near_t_y < near_t_x && ray_dir.x > 0.f && ray_dir.y < 0.f)
		{
			normal.x = -1.f;
			normal.y = 0.f;
		}
		else if (near_t_y > near_t_x && ray_dir.x > 0.f && ray_dir.y < 0.f)
		{
			normal.x = 0.f;
			normal.y = 1.f;
		}
		else if (near_t_y < near_t_x && ray_dir.x < 0.f && ray_dir.y < 0.f)
		{
			normal.x = 1.f;
			normal.y = 0.f;
		}
		else if (near_t_y > near_t_x && ray_dir.x < 0.f && ray_dir.y < 0.f)
		{
			normal.x = 0.f;
			normal.y = 1.f;
		}
		else if (near_t_y < near_t_x && ray_dir.x < 0.f && ray_dir.y > 0.f)
		{
			normal.x = 1.f;
			normal.y = 0.f;
		}
		else if (near_t_y > near_t_x && ray_dir.x < 0.f && ray_dir.y > 0.f)
		{
			normal.x = 0.f;
			normal.y = -1.f;
		}
		else
		{
			return false;
		}*/


		// ray intersects
	//}

	// find t_near and t_far;


