#ifndef BOX_HPP__
#define BOX_HPP__

#include <sprite/ASprite.hpp>
#include <memory>
// describes an axis-aligned rectangle with a velocity 
struct Box
{
	// position of top-left corner 
	float x{ 0.f }, y{ 0.f };

	// dimensions 
	float w{0.f}, h{0.f};

	// velocity 
	float vx{ 0.f }, vy{ 0.f };

	bool isBroadphaseBox{ false };

	//non-owning
	ASprite* pSpr{ nullptr };
	Box() = default;
	~Box();
	Box(const Box&) = delete;
	Box& operator=(const Box&) = delete;
	Box(Box&&) noexcept;
	Box& operator=(Box&&) noexcept;

	static void SetupBox(Box& abox, ASprite& l_asprite);
	static void SetSpriteVelocity(Box& abox);
	static void SetSpriteVelocity(Box& abox, sf::Vector2f l_vel);
	static void UpdateSpriteVelocity(Box& abox, float l_dt);
	static void UpdateSpriteVelocity(Box& abox, sf::Vector2f l_vel, float l_dt);
	static void UpdateBoxVelocity(Box& abox, sf::Vector2f l_vel, float l_dt);
	static void SetBoxVelocity(Box& abox, sf::Vector2f l_vel);
	static void AddToBoxVelocity(Box& abox, sf::Vector2f l_vel);
	static void UpdateBoxPos(Box& abox, float l_dt);
	static void SetBoxPos(Box& abox, sf::Vector2f l_pos);
	static void MoveBox(Box& abox, sf::Vector2f l_offset, float l_dt);
	static void UpdateSpritePos(Box& abox, float l_dt);
	static void SetSpritePos(Box& abox, sf::Vector2f l_pos);
	static void MoveSprite(Box& abox, sf::Vector2f l_offset, float l_dt);

	static float SweptAABB(Box& b1, Box& b2, float& normalx, float& normaly);
	static std::unique_ptr<Box> GetSweptBroadphaseBox(Box& b);
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