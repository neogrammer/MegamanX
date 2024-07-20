#include "globals.hpp"

SpriteKey::SpriteKey(SpriteType l_spriteType, SpriteName l_spriteName, AnimType l_animType)
	: type{ l_spriteType }
	, name{ l_spriteName }
	, animType{ l_animType }
{}