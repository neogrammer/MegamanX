#include "Animation.hpp"

Animation::Animation(sf::Sprite& l_spr, sf::Texture& l_tex, AnimLayoutType l_layoutType, AnimType l_type, uint32_t l_numFrames, const sf::IntRect& l_firstFrame, uint32_t l_cols, uint32_t l_rows, float l_frameDelay, float l_lingerWait, bool l_hasLeft, bool l_looping, bool l_persistent, bool l_loopStartWait, float l_loopStartWaitTime)
	: m_layoutType{ l_layoutType }
	, m_type{l_type}
	, m_numFrames{l_numFrames}
	, m_cols{l_cols}
	, m_rows{l_rows}
	, m_frameSize{l_firstFrame.getSize()}
	, m_frameDelay{sf::seconds(l_frameDelay)}
	, m_lingerTime{ sf::seconds(l_lingerWait) }
	, m_persistent{l_persistent}
	, m_looping{l_looping}
	, m_hasLeftFrames{l_hasLeft}
	, m_texture{ &l_tex  }
	, m_spr{&l_spr}
	, m_startPos{l_firstFrame.getPosition()}
	, m_loopStartWait{l_loopStartWait}
	, m_loopStartWaitTime{l_loopStartWaitTime }
{
	m_spr->setTexture(*m_texture);

	if (m_layoutType == AnimLayoutType::Vertical)
	{
		loadVStrip();
	}
	else if (m_layoutType == AnimLayoutType::Horizontal)
	{
		loadHStrip();
	}
	else
	{
		loadSquareStrip();
	}

}


void Animation::loadVStrip()
{
	m_framesRight.clear();
	m_framesRight.reserve(m_numFrames);
	for (uint32_t x = 0; x < m_rows; x++)
	{

	}

	if (m_hasLeftFrames)
	{
		m_framesLeft.clear();
		m_framesLeft.reserve(m_numFrames);
		for (uint32_t x = 0; x < m_rows; x++)
		{

		}
	}
}

void Animation::loadHStrip()
{
	m_framesRight.clear();
	m_framesRight.reserve(m_numFrames);
	for (uint32_t x = 0; x < m_cols; x++)
	{
		m_framesRight.emplace_back(sf::IntRect{ {(int)(m_startPos.x + (x * m_frameSize.x)),m_startPos.y},{m_frameSize.x,m_frameSize.y} });
	}


	if (m_hasLeftFrames)
	{
		m_framesLeft.clear();
		m_framesLeft.reserve(m_numFrames);
		for (uint32_t x = 0; x < m_cols; x++)
		{
			m_framesLeft.emplace_back(sf::IntRect{ {(int)(m_startPos.x + (x * m_frameSize.x)),m_startPos.y + m_frameSize.y},{m_frameSize.x,m_frameSize.y} });
		}
	}
}

void Animation::loadSquareStrip()
{
	m_framesRight.clear();
	m_framesRight.reserve(m_numFrames);
	for (uint32_t x = 0; x < m_rows; x++)
	{
		for (uint32_t x = 0; x < m_cols; x++)
		{

		}
	}

	if (m_hasLeftFrames)
	{
		m_framesLeft.clear();
		m_framesLeft.reserve(m_numFrames);
		for (uint32_t x = 0; x < m_rows; x++)
		{
			for (uint32_t x = 0; x < m_cols; x++)
			{

			}
		}
	}
}

void Animation::update(const sf::Time& l_dt)
{
	if (m_playing)
	{
		if (m_frameTimer.getElapsedTime() > m_frameDelay)
		{
			m_frameTimer.restart();
			animate();
		}


		m_spr->setTexture(*m_texture);
		
		
		if (m_facingRight)
		{
			m_spr->setTextureRect(m_framesRight[m_currFrame]);
		}
		else
		{
			if (m_hasLeftFrames)
				m_spr->setTextureRect(m_framesLeft[m_currFrame]);
		}
	}
}

void Animation::animate()
{
	if (m_loopStartWait)
	{
		if (m_currFrame == 0Ui64)
		{
			if (m_loopStartWaitTimer.getElapsedTime().asSeconds() > m_loopStartWaitTime)
			{
				if (m_numFrames > 1)
					m_currFrame++;
			}
		}
		else if (m_currFrame + 1 < m_numFrames - 1)
		{
			m_currFrame++;
			m_isOnLastFrame = false;

		}
		else if (m_currFrame + 1 == m_numFrames - 1)
		{
			m_currFrame++;
			m_isOnLastFrame = true;
			m_lingerTimer.restart();
		}
		else
		{
			if (m_looping)
			{
				m_loopStartWaitTimer.restart();
				m_currFrame = 0Ui64;
				m_isOnLastFrame = false;
			}
			else
			{
				if (m_persistent)
				{
					m_currFrame = m_numFrames - 1;
					m_isOnLastFrame = true;
					m_popMe = false;
				}
				else
				{

					m_currFrame = m_numFrames - 1;
					m_isOnLastFrame = true;
					if (m_lingerTimer.getElapsedTime() > m_lingerTime)
					{
						m_popMe = true;
					}
				}
			}
		}
		
	}
	else
	{
		if (m_currFrame + 1 < m_numFrames - 1)
		{
			m_currFrame++;
			m_isOnLastFrame = false;

		}
		else if (m_currFrame + 1 == m_numFrames - 1)
		{
			m_currFrame++;
			m_isOnLastFrame = true;
			m_lingerTimer.restart();
		}
		else
		{
			if (m_looping)
			{
				m_loopStartWaitTimer.restart();
				m_currFrame = 0Ui64;
				m_isOnLastFrame = false;
			}
			else
			{
				if (m_persistent)
				{
					m_currFrame = m_numFrames - 1;
					m_isOnLastFrame = true;
					m_popMe = false;
				}
				else
				{

					m_currFrame = m_numFrames - 1;
					m_isOnLastFrame = true;
					if (m_lingerTimer.getElapsedTime() > m_lingerTime)
					{
						m_popMe = true;
					}
				}
			}
		}
	}
}

AnimType Animation::GetType()
{
	return m_type;
}

void Animation::Play()
{
	m_playing = true;
	m_loopStartWaitTimer.restart();

}

void Animation::Stop()
{
	m_playing = false;
	m_currFrame = 0Ui64;


}

void Animation::Pause()
{
	m_playing = false;

}

void Animation::Resume()
{
	m_loopStartWaitTimer.restart();
	m_playing = true;

}

void Animation::Reset()
{
	m_loopStartWaitTimer.restart();
	m_currFrame = 0Ui64;

}
