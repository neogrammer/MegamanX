#include "Cfg.hpp"
#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

ResourceManager<sf::Texture, int> Cfg::textures = {};
ResourceManager<sf::Font, int> Cfg::fonts = {};
ResourceManager<sf::Music, int> Cfg::music = {};
ResourceManager<sf::SoundBuffer, int> Cfg::sounds = {};

void Cfg::Initialize()
{
    initTextures();
    initFonts();
    initMusic();
    initSounds();
 
}

void Cfg::initMusic()
{
    /*  music.load(Music::Intro, "assets/sounds/music/tetrisc.wav"); */
}

void Cfg::initSounds()
{

}

void Cfg::initTextures()
{
	textures.load((int)Textures::Bullet1, "assets/tex/projectiles/bustershot_normal.png");
	textures.load((int)Textures::Tileset1, "assets/tex/tilesets/tileset1.png");
	textures.load((int)Textures::PlayerIdle, "assets/tex/megaman/idle120x136_3_60x76.png");
	textures.load((int)Textures::PlayerRun, "assets/tex/megaman/run136x140_10_68x70.png");
	textures.load((int)Textures::PlayerTransRun, "assets/tex/megaman/trans_run135x136_1_67pt5x72.png");


}

void Cfg::initFonts()
{
   
}