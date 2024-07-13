#ifndef ANIMATION_HPP__
#define ANIMATION_HPP__
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <misc/globals.hpp>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

struct Animation
{
	friend struct AnimationMgr;

	Animation()= default;
	Animation(sf::Sprite& l_spr, sf::Texture& l_tex, AnimLayoutType l_layoutType, AnimType l_type, uint32_t l_numFrames, const sf::IntRect& l_firstFrame, uint32_t l_cols, uint32_t l_rows, float l_frameDelay, float l_lingerWait, bool l_hasLeft, bool l_looping, bool l_persistent, bool l_loopStartWait = false, float l_loopStartWaitTime = 0.f);
	~Animation()=default;

	Animation(const Animation&) = default;
	Animation& operator=(const Animation&) = default;
	Animation(Animation&&) = delete;
	Animation& operator=(Animation&&) = delete;

	void Play();
	void Stop();
	void Pause();
	void Resume();
	void Reset();
	/*bool IsPlaying();
	void SetFacingRight(bool l_facingRight);
	bool IsFacingRight();
	bool IsOnLastFrame();
	uint32_t GetCurrIdx();
	void SetCurrIdx(uint32_t l_idx);
	sf::IntRect GetCurrFrame();
	void SetFrame(const sf::IntRect& l_rect);*/
private:
	std::vector<sf::IntRect> m_framesRight = {};
	std::vector<sf::IntRect> m_framesLeft = {};
	sf::Sprite* m_spr{ nullptr };
	sf::Texture* m_texture{ nullptr };
	AnimType m_type{ AnimType::Count };
	AnimLayoutType m_layoutType{ AnimLayoutType::Count };
	uint32_t m_numFrames{ 0Ui64 };
	uint32_t m_currFrame{ 0Ui64 };
	uint32_t m_cols{ 0Ui64 };
	uint32_t m_rows{ 0Ui64 };
	sf::Time m_frameDelay{ sf::Time::Zero };
	sf::Time m_lingerTime{ sf::Time::Zero };
	sf::Clock m_frameTimer = {};
	sf::Clock m_lingerTimer = {};
	sf::Vector2i m_startPos = { 0, 0 };
	sf::Vector2i m_frameSize = { 0,0 };
	bool m_persistent{ true };
	bool m_looping{ true };
	bool m_playing{ true };
	bool m_popMe{ false };
	bool m_facingRight{ true };
	bool m_hasLeftFrames{ false };
	bool m_isOnLastFrame{ false };
	void loadVStrip();
	void loadHStrip();
	void loadSquareStrip();
	bool m_loopStartWait{ false };
	float m_loopStartWaitTime{ 0.f };
	sf::Clock m_loopStartWaitTimer{};
	void update(const sf::Time& l_dt);
	void animate();


};

#endif