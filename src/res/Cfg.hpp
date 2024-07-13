#ifndef CFG_HPP__
#define CFG_HPP__

#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include <vector>

struct Cfg
{
	Cfg() = delete;
	Cfg(const Cfg&) = delete;
	Cfg& operator=(const Cfg&) = delete;

	static void Initialize();

	// Resource Enums 
	enum class Textures : int { Tileset1, Bullet1, PlayerIdle, PlayerRun, PlayerTransRun, Count };
	enum class Fonts : int { Count };
	enum class Music : int { Count };
	enum class Sounds : int { Count };

	// resource buckets for each type of resource
	static ResourceManager<sf::Texture, int> textures;
	static ResourceManager<sf::Font, int> fonts;
	static ResourceManager<sf::Music, int> music;
	static ResourceManager<sf::SoundBuffer, int> sounds;

private:
    // initalize the resources for the entire game
    static void initFonts();
    static void initMusic();
    static void initSounds();
    static void initTextures();
};

#endif