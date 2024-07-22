#ifndef CFG_HPP__
#define CFG_HPP__

#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <action/ActionMap.hpp>
#include <vector>

struct Cfg
{
	Cfg() = delete;
	Cfg(const Cfg&) = delete;
	Cfg& operator=(const Cfg&) = delete;

	static void Initialize();

	// Resource Enums 
	enum class Textures : int { Tileset1, BGSpace, Bullet1, BirdSheetWSaw, 
		PlayerIdle, PlayerRun, PlayerTransRun, PlayerShootStand, 
		PlayerTransJump, PlayerRise, PlayerTransFall, PlayerFall, PlayerLand,
		PlayerAtlas, PlayerAtlasLeft,PlayerSheet,
		Count };
	enum class Fonts : int { Count };
	enum class Music : int { Count };
	enum class Sounds : int { Count };

	// inputs the player will be able to use
	enum PlayerInputs : int
	{
		// Keyboard mappable Keypresses
		Up, Down, Left, Right, A, X, Y, B, Start, Select, R1, L1,

		//// Joystick input, like a ps5 controller, DPad* and Axis* are handled differently than the rest of the joystick
		DPadX, DPadY, AxisX, AxisY,
		JoyA, JoyB, JoyX, JoyY, JoyR1, JoyL1, JoyStart, JoySelect
	};

	// resource buckets for each type of resource
	static ResourceManager<sf::Texture, int> textures;
	static ResourceManager<sf::Font, int> fonts;
	static ResourceManager<sf::Music, int> music;
	static ResourceManager<sf::SoundBuffer, int> sounds;

	static ActionMap<int> playerInputs;
	static float Gravity;
	static float Friction;

private:
    // initalize the resources for the entire game
    static void initFonts();
    static void initMusic();
    static void initSounds();
    static void initTextures();
	static void initPlayerInputs();
};

#endif