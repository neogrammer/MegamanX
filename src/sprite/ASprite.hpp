#ifndef ASPRITE_HPP__
#define ASPRITE_HPP__

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <misc/globals.hpp>
#include <mgmt/AnimationMgr.hpp>
#include <memory>
#include <vector>
#include <map>
#include <misc/globals.hpp>
#include <sprite/AABB.hpp>

struct ASprite : AABB
{
	friend struct SandboxStage;
	friend struct Stage;

	ASprite() = default;
	ASprite(SpriteType l_type, SpriteName l_name, sf::Texture& l_tex, sf::IntRect l_startFrame = { {0,0},{0,0} }, sf::IntRect l_bbox = { {0,0},{0,0} });
	virtual ~ASprite() = default;

	ASprite(const ASprite&);
	ASprite& operator=(const ASprite&);

	sf::Sprite& operator()();
	bool IsFacingRight();
	void processInputBase();
	void updateBase(const sf::Time& l_dt);
	void render(sf::RenderWindow& l_wnd);
	
	SpriteType getType();
	SpriteName getName();
	bool IsAlive();
	void SetAlive(bool l_alive);
	sf::IntRect GetTextureRect();
	void SetGrounded(bool l_grounded);
	bool GetGrounded();
	AnimationMgr animMgr;
	bool isSetExternal;
	void setFixedPosition(sf::Vector2f l_pos);
	bool JustJumped();
	void SetJustJumped(bool l_jumped);

	friend bool BroadphaseCheck(sf::FloatRect l_spr1, ASprite& l_spr2);
	friend sf::FloatRect GetSweptBroadphaseBox(ASprite& l_spr1, float l_dt);

	bool NeedsStageToShoot();
	void BulletWasShot();

	void BulletWasDestroyed();
	

	sf::Vector2f& vel();
	const sf::Sprite& getSpr() const;
	const sf::Vector2f& getVel() const;
	void setVelocity(const sf::Vector2f& l_vel);
	void accelerate(sf::Vector2f l_acceleration, float l_dt);
	void tickPos(float l_dt);
	void setPos(sf::Vector2f l_pos);
	void move(sf::Vector2f l_offset, float l_dt);
									const float					   bTop() const;
									const float					  bLeft() const;
									const float					 bRight() const;
									const float					bBottom() const;
									const sf::Vector2f			  boxTL() const;
									const sf::Vector2f			  boxBR() const;
								 sf::FloatRect getBoxGlobalBounds();
	void setBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight, const sf::IntRect& l_rect);
	sf::IntRect getBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight);
	const sf::IntRect& getCurrBoxRect() const;

	void collideAABB(AABB other);
	void updatePhysics(float gravity_, float friction_);
	std::vector<ASprite*> blockXTiles{};
	float blockVelX_{};
	float blockPosYBefore_{};
	float blockPosYAfter_{};
	ASprite* closestTile_{ nullptr };
	bool falling_{ false };
	bool inFreeFall_{ false };
	bool fallFastFlag_{ false };
	bool blockX_{ false };
	sf::Sprite spr_;
	bool jumping_{ false };
	bool justJumped_{ true };
	bool collidingOnX_{ false };
	bool alive_{ true };
	SpriteType type_{ SpriteType::Count };
	SpriteName name_{ SpriteName::Count };
	bool grounded_{ false };
	bool affectedByGravity_{ false };
	bool movingRight_{ false };
	bool needsStageToShoot_{ false };
	bool movingLeft_{ false };
	bool facingRight_{ true };
	bool wasFacingRight_{ true };
	bool shooting_{ false };
	sf::Time shootDelay_{ sf::seconds(0.08f) };
	sf::Time shootElapsed_{ sf::Time::Zero };

	int maxBullets_{ 5 };
	int numBullets_{ 0 };

	bool invincible_{ false };
	sf::Time invincibleTime_ = sf::Time::Zero;


	sf::Vector2f vel_;
	sf::Time gameTime_;




	// std::unordered_map<AnimType, Animation> animMap_;

	virtual void processInput(/* const Command& l_cmd */) = 0;
	virtual void update(const sf::Time& l_dt) = 0;
	virtual bool IsShooting();
	virtual void Shoot();

private:
	
	void updatePosition();
	void updateTexRect();
};




#endif