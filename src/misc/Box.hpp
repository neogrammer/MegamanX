#ifndef BOX_HPP__
#define BOX_HPP__

#include <memory>
#include <utility>
#include <SFML/System/Vector2.hpp>
struct ASprite;
// describes an axis-aligned rectangle with a velocity 
struct Box
{

	ASprite* pSpr;
	// position of top-left corner 
	// points to the actual sprite position
	float* sprXPos;
	float* sprYPos;

	// offset
	float x{ 0 }, y{ 0 };

	// dimensions 
	float w{ 0.f }, h{ 0.f };

	// velocity 
	float* vx;
	float* vy;

	bool isBroadphaseBox{ false };

	//non-owning

	Box(); 
	~Box();
	Box(const Box& o) = default;
	Box& operator=(const Box& o) = default;
	Box(Box&&) noexcept;
	Box& operator=(Box&&) noexcept;

	sf::Vector2f getTLPt();
	sf::Vector2f getTLOffset();

	sf::Vector2f getCenterPt();
	sf::Vector2f getCenterOffset();

	sf::Vector2f getBRPt();
	sf::Vector2f getBROffset();

	sf::Vector2f getHSize();
	sf::Vector2f getSize();

	static void SetupBox(Box& abox, ASprite& l_asprite);
	static void SetVelocity(Box& abox, sf::Vector2f l_vel);
	static void UpdateVelocity(Box& abox, sf::Vector2f l_acceleration, float l_dt);
	static void AddToVelocity(Box& abox, sf::Vector2f l_vel);
	static void UpdatePos(Box& abox, float l_dt);
	static void SetPos(Box& abox, sf::Vector2f l_pos);
	static void Move(Box& abox, sf::Vector2f l_offset, float l_dt);

	static float SweptAABB(Box& b1, Box& b2, float& normalx, float& normaly);
	static std::unique_ptr<Box> GetSweptBroadphaseBox(Box& b, float l_dt);
	static bool BroadphaseCheck(Box& b1, Box& b2);
	static void Bounce(Box& b1, float l_remTime, float& normalx, float& normaly);
	static void Slide(Box& box, float l_remTime, float& normalx, float& normaly);
	static void Push(Box& box, float l_remTime, float& normalx, float& normaly);

	
};

/*

collision detection and resolution
*********************************
enum class CollisionStrategy
{
	Bounce,
	Slide,
	Push
};


bool CollisionMgr::CheckCollisionAndResolve(ASprite& l_dynamicSpr, ASprite& l_staticSpr, CollisionStrategy l_collideStrat)
{
	Box box{};
	Box::SetupBox(box, l_dynamicSpr);
	Box block{};
	Box::SetupBox(block, l_staticSpr);

	std::unique_ptr<Box> broadphasebox = { std::move(Box::GetSweptBroadphaseBox(box)) };

	if (broadphasebox != nullptr)
	{
		if (Box::BroadphaseCheck(*broadphasebox, block))
		{
			float normalx, normaly;
			float collisiontime = Box::SweptAABB(box, block, normalx, normaly);
			if (collisiontime > 0.f && collisiontime <= 1.f)
			{
				Box::UpdateBoxPos(box, collisiontime);

				float remainingtime = 1.0f - collisiontime;

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

				return true;
			}


		}

	}
	return false;
}

*************************************

*/

#endif