#include <sprite/ASprite.hpp>

#ifndef BULLET_HPP__
#define BULLET_HPP__

struct Bullet : ASprite
{

	Bullet();
	Bullet(sf::Texture& l_tex, bool l_friendly = true);
	~Bullet() override = default;

	Bullet(const Bullet&);
	Bullet& operator=(const Bullet&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;
	bool GetFriendly();
private:
	bool m_friendly{};
};

#endif