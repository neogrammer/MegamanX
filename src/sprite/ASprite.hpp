#ifndef ASPRITE_HPP__
#define ASPRITE_HPP__

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <misc/globals.hpp>
#include <mgmt/AnimationMgr.hpp>


struct ASprite
{
	

	ASprite() = default;
	ASprite(SpriteType l_type, SpriteName l_name, sf::Texture& l_tex);
	virtual ~ASprite() = default;

	ASprite(const ASprite&);
	ASprite& operator=(const ASprite&);

	sf::Sprite& operator()();
	bool IsFacingRight();
	void processInputBase();
	void updateBase(const sf::Time& l_dt);
	void render(sf::RenderWindow& l_wnd);
	sf::Vector2f& vel();
	SpriteType getType();
	SpriteName getName();
	bool IsAlive();
	void SetAlive(bool l_alive);
	sf::IntRect GetRect();
	void SetGrounded(bool l_grounded);
	AnimationMgr animMgr;
	bool isSetExternal{ false };
	void setFixedPosition(sf::Vector2f l_pos);

	static void SyncSpriteToAnim(ASprite& l_spr);
	float oldHeight{ 0.f };

	bool NeedsStageToShoot();
	void BulletWasShot();

	void BulletWasDestroyed();

protected:
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
	sf::Time shootDelay_{sf::seconds(0.08f)};
	sf::Time shootElapsed_{ sf::Time::Zero };

	int maxBullets_{ 5 };
	int numBullets_{ 0 };

	bool invincible_{ false };
	sf::Time invincibleTime_ = sf::Time::Zero;

	sf::Sprite spr_;
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