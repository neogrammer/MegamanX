#include "AnimationMgr.hpp"

#include <iostream>

void AnimationMgr::AddAnimation(sf::Sprite& l_spr, sf::Texture& l_tex, AnimLayoutType l_layoutType, AnimType l_type, uint32_t l_numFrames, const sf::IntRect& l_firstFrame, uint32_t l_cols, uint32_t l_rows, float l_frameDelay, float l_lingerWait, bool l_hasLeft, bool l_looping, bool l_persistent, bool l_loopStartWait, float l_loopStartWaitTime)
{

	m_animMap.insert({ l_type, { l_spr, l_tex,  l_layoutType, l_type, l_numFrames,  l_firstFrame, l_cols, l_rows,  l_frameDelay, l_lingerWait, l_hasLeft, l_looping,l_persistent, l_loopStartWait, l_loopStartWaitTime} });

		m_currAnim = &m_animMap[l_type];
		m_fallback = m_currAnim;
		l_spr.setTextureRect(l_firstFrame);
}

void AnimationMgr::SwitchAnimation(AnimType l_type, AnimType l_fallback)
{
	if (l_type != m_currAnim->m_type)
	{
		m_currAnim = &m_animMap[l_type];

		if (l_fallback != AnimType::Count)
		{
			m_fallback = &m_animMap[l_fallback];
		}
	}
}

void AnimationMgr::Update(const sf::Time& l_dt)
{

	if (!m_currAnim)
	{
		return;
	}
	if (m_currAnim->m_isOnLastFrame && m_currAnim->m_popMe && m_fallback != nullptr)
	{
		m_currAnim->m_facingRight = m_facingRight;

		m_currAnim->m_popMe = false;
		m_currAnim->Reset();
		m_currAnim = &m_animMap[m_fallback->m_type];
		m_fallback = nullptr;
		m_currAnim->Reset();
		m_currAnim->Play();



	}
	else
	{
		m_currAnim->m_facingRight = m_facingRight;

		m_currAnim->update(l_dt);
	}
}

void AnimationMgr::SetFacingRight(bool l_facingRight)
{
	m_facingRight = l_facingRight;
}

bool AnimationMgr::IsFacingRight()
{
	return m_facingRight;
}

bool AnimationMgr::currentIsOnLastFrame()
{
	return m_currAnim->m_isOnLastFrame;
}

bool AnimationMgr::currIsReadyToPop()
{
	return m_currAnim->m_popMe;
}

Animation* AnimationMgr::currFallback()
{
	return m_fallback;
}

AnimType AnimationMgr::getCurrType()
{
	if (!m_currAnim)
	{
		return AnimType::Count;
	}
    return m_currAnim->GetType();
}

uint32_t AnimationMgr::getCurrFrameIdx()
{
	if (!m_currAnim)
	{
		return 0Ui64;
	}
    return m_currAnim->m_currFrame;
}

void AnimationMgr::setBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight, sf::IntRect l_box)
{
	if (m_animMap.find(l_type) != m_animMap.end())
	{
		

		if (l_facingRight)
		{

			if (l_index >= 0 && l_index < m_animMap.at(l_type).m_boxesRight.size())
				m_animMap[l_type].m_boxesRight[l_index] = l_box;
			else
				std::cout << "index out of range of m_boxesRight vector" << std::endl;
		}
		else
		{
			if (l_index >= 0 && l_index < m_animMap.at(l_type).m_boxesLeft.size())
				m_animMap[l_type].m_boxesLeft[l_index] = l_box;
			else
				std::cout << "index out of range of m_boxesLeft vector" << std::endl;
		}
	}
	std::cout << "AnimType does not exist in mgr" << std::endl;
}

sf::IntRect AnimationMgr::getBoxRect(AnimType l_type, uint32_t l_index, bool l_facingRight)
{
	if (m_animMap.find(l_type) != m_animMap.end())
	{


		if (l_facingRight)
		{

			if (l_index >= 0 && l_index < m_animMap.at(l_type).m_boxesRight.size())
				return m_animMap[l_type].m_boxesRight.at(l_index);
			else
				std::cout << "index out of range of m_boxesRight vector" << std::endl;
		}
		else
		{
			if (l_index >= 0 && l_index < m_animMap.at(l_type).m_boxesLeft.size())
				return m_animMap[l_type].m_boxesLeft.at(l_index);
			else
				std::cout << "index out of range of m_boxesLeft vector" << std::endl;
		}
		throw std::runtime_error("BadCall::AnimationMgr::getBoxRect");
	}
	std::cout << "AnimType does not exist in mgr" << std::endl;
	throw std::runtime_error("BadCall::AnimationMgr::getBoxRect");

}

const sf::IntRect& AnimationMgr::getCurrBox() const 
{
	
	if (this->m_facingRight)
	{
		return m_currAnim->m_boxesRight[m_currAnim->m_currFrame];
	}
	else
	{
		return m_currAnim->m_boxesLeft[m_currAnim->m_currFrame];
	}
	
}

void AnimationMgr::copyDataTo(AnimationMgr& l_mgr, sf::Sprite& l_spr)
{
	l_mgr.m_animMap.clear();
	for (auto& a : m_animMap)
	{
		l_mgr.m_animMap.insert_or_assign(a.first, a.second);
		l_mgr.m_animMap[a.first].m_spr = &l_spr;
	}
}
