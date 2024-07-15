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
	ASprite(SpriteType l_type, sf::Texture& l_tex);
	virtual ~ASprite() = default;

	ASprite(const ASprite&);
	ASprite& operator=(const ASprite&);

	sf::Sprite& operator()();

	void processInputBase();
	void updateBase(const sf::Time& l_dt);
	void render(sf::RenderWindow& l_wnd);
	sf::Vector2f& vel();
	SpriteType getType();
	bool IsAlive();
	void SetAlive(bool l_alive);
	sf::IntRect GetRect();
	void SetGrounded(bool l_grounded);
	AnimationMgr animMgr;
	bool isSetExternal{ false };
	void setFixedPosition(sf::Vector2f l_pos);

protected:
	bool alive_{ true };
	SpriteType type_{ SpriteType::Count };
	bool grounded_{ false };
	bool affectedByGravity_{ false };
	bool movingRight_{ false };
	bool movingLeft_{ false };
	bool facingRight_{ true };

	sf::Sprite spr_;
	sf::Vector2f vel_;
	sf::Time gameTime_;


	// std::unordered_map<AnimType, Animation> animMap_;

	virtual void processInput(/* const Command& l_cmd */) = 0;
	virtual void update(const sf::Time& l_dt) = 0;
	

private:
	
	void updatePosition();
	void updateTexRect();
};

#endif