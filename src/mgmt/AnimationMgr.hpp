#ifndef ANIMATIONMGR_HPP__
#define ANIMATIONMGR_HPP__
#include <unordered_map>
#include <misc/globals.hpp>
#include <sprite/Animation.hpp>
struct AnimationMgr
{
	void AddAnimation(sf::Sprite& l_spr, sf::Texture& l_tex, AnimLayoutType l_layoutType, AnimType l_type, uint32_t l_numFrames, const sf::IntRect& l_firstFrame, uint32_t l_cols, uint32_t l_rows, float l_frameDelay, float l_lingerWait, bool l_hasLeft, bool l_looping, bool l_persistent, bool l_loopStartWait = false, float l_loopStartWaitTime = 0.f);
	void SwitchAnimation(AnimType l_type, AnimType l_fallback = AnimType::Count);
	void Update(const sf::Time& l_dt);
	void SetFacingRight(bool l_facingRight);
	bool IsFacingRight();

	bool currentIsOnLastFrame();
	bool currIsReadyToPop();
	Animation* currFallback();
	AnimType getCurrType();
	uint32_t getCurrFrameIdx();
	void setBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight, sf::IntRect l_box, sf::IntRect l_leftRect = { {0,0},{0,0} });
	sf::IntRect getBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight);
	const sf::IntRect& getCurrBox() const;
	void copyDataTo(AnimationMgr& l_mgr, sf::Sprite& l_spr);
private:
	std::unordered_map<AnimType, Animation> m_animMap = {};
	Animation* m_currAnim{ nullptr };
	Animation* m_fallback{ nullptr };
	bool m_facingRight{ true };

	

	
};

#endif