#ifndef GLOBALS_HPP__
#define GLOBALS_HPP__
#include <SFML/System/Time.hpp>
#include <string>
#include <SFML/System/Vector2.hpp>

namespace g
{
	constexpr unsigned int WW = 1600;
	constexpr unsigned int WH = 900;

	const std::string GNAME = "MegamanX";

	const sf::Time FPS60 = sf::seconds( 1.f / 60.f );
}

enum class SpriteType : int
{
	Actor,
	Enemy,
	Projectile,
	Tile,
	Count
};

enum class SpriteName : int
{
	Player,
	BuzzBird,
	NormalTile,
	BusterBullet,
	BuzzSaw,
	Count
};

enum class AnimLayoutType
{
	Horizontal,
	Vertical,
	Square,
	Count
};

enum class AnimType : int
{
	Idle,
	Run,
	TransRun,
	TransJump,
	Rise,
	TransFall,
	Fall,
	Land,
	ShootStand,
	None,
	Count
};

enum class GameStateType
{
	Splash,
	Title,
	StageSelect,
	Stage,
	Load,
	Menu,
	Over,
	Sandbox,
	Count
};

enum class StageType
{
	Intro,
	Sandbox,
	Count
};

struct rect
{
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f vel;
};

#endif