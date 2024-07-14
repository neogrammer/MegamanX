#include "CollisionMgr.hpp"
#include <sprite/sprites/Bullet.hpp>
#include <misc/globals.hpp>
sf::FloatRect CollisionMgr::m_currOverlap = { { 0.f, 0.f }, { 0.f,0.f } };
sf::FloatRect CollisionMgr::m_prevOverlap = { { 0.f, 0.f }, { 0.f,0.f } };


void CollisionMgr::CheckCollisions(ASprite& l_sprA, std::vector<std::shared_ptr<ASprite>> l_sprVec)
{
	for (auto& s : l_sprVec)
	{
		if (&l_sprA == s.get())
		{
			continue;
		}
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
	
	if (m_prevOverlap.width < 0 && m_prevOverlap.height < 0)
	{
		l_spr.vel().x = 0.f;
		l_spr().setPosition(l_spr().getPosition().x - m_currOverlap.width, l_spr().getPosition().y);
	}
	else if (m_prevOverlap.width < 0)
	{
		l_spr.vel().x = 0.f;
		l_spr().setPosition(l_spr().getPosition().x - m_currOverlap.width, l_spr().getPosition().y);
	}
	else
	{
		l_spr.vel().y = 0.f;
		l_spr().setPosition(l_spr().getPosition().x, l_spr().getPosition().y - m_currOverlap.height);
		l_spr.SetGrounded(true);
	}
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
