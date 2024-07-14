#include "AnimationMgr.hpp"

void AnimationMgr::AddAnimation(sf::Sprite& l_spr, sf::Texture& l_tex, AnimLayoutType l_layoutType, AnimType l_type, uint32_t l_numFrames, const sf::IntRect& l_firstFrame, uint32_t l_cols, uint32_t l_rows, float l_frameDelay, float l_lingerWait, bool l_hasLeft, bool l_looping, bool l_persistent, bool l_loopStartWait, float l_loopStartWaitTime)
{
	m_animMap.insert({ l_type, { l_spr, l_tex,  l_layoutType, l_type, l_numFrames,  l_firstFrame, l_cols, l_rows,  l_frameDelay, l_lingerWait, l_hasLeft, l_looping,l_persistent, l_loopStartWait, l_loopStartWaitTime} });
	if (!m_currAnim)
	{
		m_currAnim = &m_animMap[l_type];
		m_fallback = m_currAnim;
		l_spr.setTextureRect(l_firstFrame);
	}
}

void AnimationMgr::SwitchAnimation(AnimType l_type, AnimType l_fallback)
{
	m_currAnim = &m_animMap[l_type];

	if (l_fallback != AnimType::Count)
	{
		m_fallback = &m_animMap[l_fallback];
	}
}

void AnimationMgr::Update(const sf::Time& l_dt)
{
	if (!m_currAnim)
	{
		return;
	}
	m_currAnim->m_facingRight = m_facingRight;

	m_currAnim->update(l_dt);
	
	if (m_currAnim->m_popMe)
	{
		m_currAnim->m_popMe = false;
		if (!m_fallback)
		{
			return;
		}
		m_currAnim = m_fallback;
		m_fallback = nullptr;
	}
}

void AnimationMgr::SetFacingRight(bool l_facingRight)
{
	m_facingRight = l_facingRight;
}
