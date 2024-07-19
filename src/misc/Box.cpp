#include "Box.hpp"
#include <misc/aabb.hpp>
#include <res/Cfg.hpp>
#include <iostream>
#include <limits>
void Box::SetupBox(Box& abox, ASprite& l_asprite)
{
	if (abox.pSpr == nullptr)
	{
		abox.pSpr = &l_asprite;

		AABB aabb = Cfg::bboxDB.getAABB(*abox.pSpr);
		if (aabb.IsNone())
		{
			aabb = AABB{ { l_asprite().getOrigin() },{(sf::Vector2f)l_asprite.GetRect().getSize()}};
			
		}

		float cx_ = aabb.LocalCenter().x;
		float cy_ = aabb.LocalCenter().y;

		float sx_ = aabb.GetSize().x;
		float sy_ = aabb.GetSize().y;

		float vx_ = l_asprite.vel().x;
		float vy_ = l_asprite.vel().y;

		abox.x = aabb.TopLeftFromCenter().x + l_asprite().getPosition().x - l_asprite().getOrigin().x;
		abox.y = aabb.TopLeftFromCenter().y + l_asprite().getPosition().y - l_asprite().getOrigin().y;

		abox.w = aabb.GetSize().x;
		abox.h = aabb.GetSize().y;

		abox.vx = vx_;
		abox.vy = vy_;

	}
	else
	{
		std::cout << "Box already has an ASprite connected to it!" << std::endl;
		return;
	}
}

void Box::SetSpriteVelocity(Box& abox)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.pSpr->vel() = { abox.vx, abox.vy };
}
void Box::SetSpriteVelocity(Box& abox, sf::Vector2f l_vel)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.pSpr->vel() = { l_vel.x, l_vel.y };
}
void Box::UpdateSpriteVelocity(Box& abox, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.pSpr->vel() = { abox.pSpr->vel().x + abox.vx * l_dt, abox.pSpr->vel().y + abox.vy * l_dt};
}
void Box::UpdateSpriteVelocity(Box& abox,sf::Vector2f l_vel, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.pSpr->vel() = { abox.pSpr->vel().x + l_vel.x * l_dt, abox.pSpr->vel().y + l_vel.y * l_dt };
}

void Box::UpdateBoxVelocity(Box& abox, sf::Vector2f l_vel, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.vx += l_vel.x * l_dt;
	abox.vy += l_vel.y * l_dt;
}
void Box::SetBoxVelocity(Box& abox, sf::Vector2f l_vel)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.vx = l_vel.x;
	abox.vy = l_vel.y;
}

void Box::AddToBoxVelocity(Box& abox, sf::Vector2f l_vel)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.vx += l_vel.x;
	abox.vy += l_vel.y;
}


void Box::UpdateBoxPos(Box& abox, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.x += abox.vx * l_dt;
	abox.y += abox.vy * l_dt;
}

void Box::SetBoxPos(Box& abox, sf::Vector2f l_pos)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.x = l_pos.x;
	abox.y = l_pos.y;
}

void Box::MoveBox(Box& abox, sf::Vector2f l_offset, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	abox.x += l_offset.x * l_dt;
	abox.y += l_offset.y * l_dt;
}




void Box::UpdateSpritePos(Box& abox, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	AABB old = Cfg::bboxDB.getAABB(*abox.pSpr);
	float oldDiffX = (*abox.pSpr)().getOrigin().x - old.LocalCenter().x;
	float oldDiffY = (*abox.pSpr)().getOrigin().y - old.LocalCenter().y;

	float newCenterX = abox.x + abox.w / 2.f;
	float newCenterY = abox.y + abox.h / 2.f;

	float newDiffX = (*abox.pSpr)().getOrigin().x - newCenterX;
	float newDiffY = (*abox.pSpr)().getOrigin().y - newCenterY;

	float ox = oldDiffX - newDiffX;
	float oy = oldDiffY - newDiffY;

	MoveSprite(abox, { ox,oy }, l_dt);

	//(*abox.pSpr)().setPosition({ (*abox.pSpr)().getPosition().x + ox, (*abox.pSpr)().getPosition().y + oy});
}

void Box::SetSpritePos(Box& abox, sf::Vector2f l_pos)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	(*abox.pSpr)().setPosition({ l_pos });
}

void Box::MoveSprite(Box& abox, sf::Vector2f l_offset, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;
		return;
	}
	(*abox.pSpr)().move({ l_offset.x * l_dt, l_offset.y * l_dt });
}

float Box::SweptAABB(Box& b1, Box& b2, float& normalx, float& normaly)
{
	if (b1.pSpr == nullptr || b2.pSpr == nullptr)
	{
		std::cout << "Returning no collision: SweptAABB(b1,b2..) not possible! " << ((!b1.pSpr) ? std::string("| b1 missing ASprite | ") : std::string("")) << ((!b2.pSpr) ? std::string("| b2 missing ASprite |") : std::string("")) << std::endl;
		return 1.0f;
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
		if ( b1.x + b1.w < b2.x  || b1.x > b2.x + b2.w) return 1.0f;
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

void Box::Bounce(Box& b1, float l_remTime, float& normalx, float& normaly)
{
	if (b1.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;
		return;
	}
	b1.vx *= l_remTime;
	b1.vy *= l_remTime;
	if (abs(normalx) > 0.0001f) b1.vx = -b1.vx;
	if (abs(normaly) > 0.0001f) b1.vy = -b1.vy;

	SetSpriteVelocity(b1);
	

}

void Box::Slide(Box& box, float l_remTime, float& normalx, float& normaly)
{
	if (box.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;
		return;
	}
	// slide 
	float dotprod = (box.vx * normaly + box.vy * normalx) * l_remTime;
	SetBoxVelocity(box, { dotprod * normaly, dotprod * normalx });
}

void Box::Push(Box& box, float l_remTime, float& normalx, float& normaly)
{
	if (box.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;
		return;
	}
	// push 
	float magnitude = sqrt((box.vx * box.vx + box.vy * box.vy)) * l_remTime;
	float dotprod = box.vx * normaly + box.vy * normalx;

	if (dotprod > 0.0f) dotprod = 1.0f;
	else if (dotprod < 0.0f) dotprod = -1.0f;

	SetBoxVelocity(box, { dotprod * normaly * magnitude , dotprod * normalx * magnitude });
}


std::unique_ptr<Box> Box::GetSweptBroadphaseBox(Box& b, float l_dt)
{
	if (b.pSpr == nullptr || b.isBroadphaseBox)
	{
		std::cout << "Invalid Broadphase Box Setup" << std::endl;
		return nullptr;
	}
	std::unique_ptr<Box> broadphasebox = std::make_unique<Box>();

	broadphasebox->x = b.vx > 0 ? b.x : b.x + b.vx * l_dt;
	broadphasebox->y = b.vy > 0 ? b.y : b.y + b.vy * l_dt;
	broadphasebox->w = b.vx > 0 ? b.vx * l_dt + b.w : b.w - b.vx * l_dt;
	broadphasebox->h = b.vy > 0 ? b.vy * l_dt + b.h : b.h - b.vy * l_dt;
	broadphasebox->pSpr = b.pSpr;
	broadphasebox->isBroadphaseBox = true;
	return std::move(broadphasebox);
}


bool Box::BroadphaseCheck(Box& b1, Box& b2)
{
	return !(b1.x + b1.w < b2.x || b1.x > b2.x + b2.w || b1.y + b1.h < b2.y || b1.y > b2.y + b2.h);
}


Box::~Box()
{
	pSpr = nullptr;
}

Box::Box(Box&& o) noexcept
	: x{std::move(o.x)}
	, y{ std::move(o.y) }
	, w{ std::move(o.w) }
	, h{ std::move(o.h) }
	, vx{ std::move(o.vx) }
	, vy{ std::move(o.vy) }
	, isBroadphaseBox{ std::move(o.isBroadphaseBox) }
	, pSpr{ o.pSpr }
{
	o.pSpr = nullptr;
	//o safe to destruct
}

Box& Box::operator=(Box&& o) noexcept
{
	this->x = { std::move(o.x) };
	this->y = { std::move(o.y) };
	this->w = { std::move(o.w) };
	this->h = { std::move(o.h) };
	this->vx = { std::move(o.vx) };
	this->vy = { std::move(o.vy) };
	this->pSpr = o.pSpr;
	this->isBroadphaseBox = { std::move(o.isBroadphaseBox) };
	o.pSpr = nullptr;
	//o safe to destruct

	return *this;
}
