#include "Box.hpp"
#include <misc/aabb.hpp>
#include <res/Cfg.hpp>
#include <iostream>
#include <limits>
#include <sprite/ASprite.hpp>
void Box::SetupBox(Box& abox, ASprite& l_asprite)
{
	abox.sprXPos = const_cast<float*>(&l_asprite().getPosition().x);
	abox.sprYPos = const_cast<float*>(&l_asprite().getPosition().y);


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

		// width and height are variable locally
		abox.w = aabb.GetSize().x;
		abox.h = aabb.GetSize().y;

		abox.vx = &l_asprite.vel().x;
		abox.vy = &l_asprite.vel().y;

		abox.isBroadphaseBox = false;
	}
	else
	{
		std::cout << "Box already has an ASprite connected to it!" << std::endl;
		return;
	}
}

void Box::SetVelocity(Box& abox, sf::Vector2f l_vel)
{
	if (abox.vx && abox.vy)
	*abox.vx = l_vel.x;
	*abox.vy = l_vel.y;
}
void Box::UpdateVelocity(Box& abox, sf::Vector2f l_acceleration, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	*abox.vx += l_acceleration.x * l_dt;
	*abox.vy += l_acceleration.y * l_dt;

}
void Box::AddToVelocity(Box& abox, sf::Vector2f l_vel)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	*abox.vx += l_vel.x;
	*abox.vy += l_vel.y;
}

void Box::UpdatePos(Box& abox, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	*abox.sprXPos += *abox.vx * l_dt;
	*abox.sprYPos += *abox.vy * l_dt;


}
void Box::SetPos(Box& abox, sf::Vector2f l_pos)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}

	*abox.sprXPos = l_pos.x - (*abox.pSpr)().getOrigin().x + abox.x;
	*abox.sprYPos = l_pos.y - (*abox.pSpr)().getOrigin().y + abox.y;
}
void Box::Move(Box& abox, sf::Vector2f l_offset, float l_dt)
{
	if (abox.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;

		return;
	}
	*abox.sprXPos += l_offset.x * l_dt;
	*abox.sprYPos += l_offset.y * l_dt;
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
	if (*b1.vx > 0.0f)
	{
		xInvEntry = b2.x - (b1.x + b1.w);
		xInvExit = (b2.x + b2.w) - b1.x;
	}
	else
	{
		xInvEntry = (b2.x + b2.w) - b1.x;
		xInvExit = b2.x - (b1.x + b1.w);
	}

	if (*b1.vy > 0.0f)
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

	if (*b1.vx == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / *b1.vx;
		xExit = xInvExit / *b1.vx;
	}

	if (*b1.vy == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / *b1.vy;
		yExit = yInvExit / *b1.vy;
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
	*b1.vx *= l_remTime;
	*b1.vy *= l_remTime;
	if (abs(normalx) > 0.0001f) *b1.vx = -(*b1.vx);
	if (abs(normaly) > 0.0001f) *b1.vy = -(*b1.vy);
	
}

void Box::Slide(Box& box, float l_remTime, float& normalx, float& normaly)
{
	if (box.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;
		return;
	}
	// slide 
	float dotprod = (*box.vx * normaly + *box.vy * normalx) * l_remTime;
	SetVelocity(box, { dotprod * normaly, dotprod * normalx });
}

void Box::Push(Box& box, float l_remTime, float& normalx, float& normaly)
{
	if (box.pSpr == nullptr)
	{
		std::cout << "Need to setup the Box first with a ASprite!" << std::endl;
		return;
	}
	// push 
	float magnitude = sqrt((*box.vx * *box.vx + *box.vy * *box.vy)) * l_remTime;
	float dotprod = *box.vx * normaly + *box.vy * normalx;

	if (dotprod > 0.0f) dotprod = 1.0f;
	else if (dotprod < 0.0f) dotprod = -1.0f;

	AddToVelocity(box, { dotprod * normaly * magnitude , dotprod * normalx * magnitude });
}


std::unique_ptr<Box> Box::GetSweptBroadphaseBox(Box& b, float l_dt)
{
	if (b.pSpr == nullptr || b.isBroadphaseBox || !b.sprXPos || !b.sprYPos || !b.vx || !b.vy)
	{
		std::cout << "Invalid Broadphase Box Setup" << std::endl;
		return nullptr;
	}

	std::unique_ptr<Box> broadphasebox = std::make_unique<Box>();
	broadphasebox->pSpr = b.pSpr;
	
	broadphasebox->sprXPos = const_cast<float*>(&(*b.pSpr)().getPosition().x);
	broadphasebox->sprYPos = const_cast<float*>(&(*b.pSpr)().getPosition().y);
	broadphasebox->vx = &b.pSpr->vel().x;
	broadphasebox->vy = &b.pSpr->vel().y;
	broadphasebox->x = b.x;
	broadphasebox->y = b.y;

	*broadphasebox->sprXPos = *b.vx > 0 ? *b.sprXPos : *b.sprXPos + *b.vx * l_dt;
	*broadphasebox->sprYPos = *b.vx > 0 ? *b.sprYPos : *b.sprYPos + *b.vy * l_dt;

	
	broadphasebox->w = *b.vx > 0 ? *b.vx * l_dt + b.w : b.w - *b.vx * l_dt;
	broadphasebox->h = *b.vy > 0 ? *b.vy * l_dt + b.h : b.h - *b.vy * l_dt;

	broadphasebox->isBroadphaseBox = true;
	return std::move(broadphasebox);
}


bool Box::BroadphaseCheck(Box& b1, Box& b2)
{
	if (b1.pSpr == nullptr || b2.pSpr == nullptr || !b1.sprXPos || !b1.sprYPos
		|| !b2.sprXPos || !b2.sprYPos)
	{
		std::cout << "Invalid Broadphase Box Setup, noSprites connected at the check" << std::endl;
		return false;
	}
	return !(b1.getBRPt().x < b2.getTLPt().x || b1.getTLPt().x > b2.getBRPt().x || b1.getBRPt().y < b2.getTLPt().y || b1.getTLPt().y > b2.getBRPt().y);
}


Box::Box()
	: pSpr{nullptr}
	, sprXPos{nullptr}
	, sprYPos{nullptr}
	, vx{ nullptr }
	, vy {nullptr }
{
}

Box::~Box()
{
	sprXPos = nullptr;
	sprYPos = nullptr;
	pSpr = nullptr;
	vx = nullptr;
	vy = nullptr;
}

Box::Box(Box&& o) noexcept
	: sprXPos{o.sprXPos}
	, sprYPos{o.sprYPos}
	, x{std::move(o.x)}
	, y{ std::move(o.y) }
	, w{ std::move(o.w) }
	, h{ std::move(o.h) }
	, vx{ o.vx }
	, vy{ o.vy }
	, isBroadphaseBox{ std::move(o.isBroadphaseBox) }
	, pSpr{ o.pSpr }
{
	o.pSpr = nullptr;
	o.sprXPos = nullptr;
	o.sprYPos = nullptr;
	o.vx = nullptr;
	o.vy = nullptr;
	//o safe to destruct
}

Box& Box::operator=(Box&& o) noexcept
{
	this->x = { std::move(o.x) };
	this->y = { std::move(o.y) };
	this->w = { std::move(o.w) };
	this->h = { std::move(o.h) };
	this->vx = { o.vx };
	this->vy = { o.vy };
	this->sprXPos = o.sprXPos;
	this->sprYPos = o.sprYPos;
	this->pSpr = o.pSpr;
	this->isBroadphaseBox = { std::move(o.isBroadphaseBox) };
	o.pSpr = nullptr;
	o.sprXPos = nullptr;
	o.sprYPos = nullptr;
	o.vx = nullptr;
	o.vy = nullptr;
	//o safe to destruct

	return *this;
}

sf::Vector2f Box::getTLPt()
{
	if (pSpr == nullptr || !sprXPos || !sprYPos)
	{
		std::cout << "Error getting top left point of a box, no Sprite connected! Returning {-1.0f,-1.0f}" << std::endl;
		return { -1.f,-1.f };
	}

	return  { *sprXPos - (*pSpr)().getOrigin().x + this->x,  *sprYPos - (*pSpr)().getOrigin().y + this->y };
}

sf::Vector2f Box::getTLOffset()
{


	return  { this->x, this->y };
}

sf::Vector2f Box::getCenterPt()
{
	return { getTLPt().x + this->w / 2.f, getTLPt().y + this->h / 2.f };
}

sf::Vector2f Box::getCenterOffset()
{
	return { this->x + this->w / 2.f,  this->y + this->h / 2.f };
}

sf::Vector2f Box::getBRPt()
{
	return { getTLPt().x + this->w, getTLPt().y + this->h };
}

sf::Vector2f Box::getBROffset()
{
	return { this->getCenterOffset().x + (this->w / 2.f), this->getCenterOffset().y + (this->h / 2.f)};
}

sf::Vector2f Box::getHSize()
{
	return { w / 2.f, h / 2.f };
}

sf::Vector2f Box::getSize()
{
	return { w, h };
}
