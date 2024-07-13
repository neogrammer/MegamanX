#include "Wndw.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <misc/globals.hpp>
Wndw::Wndw()
	: m_wnd{ {g::WW,g::WH,32U}, g::GNAME, sf::Style::Close }
{
}

void Wndw::Fullscreen(bool l_full)
{
	if (m_full != l_full)
	{
		if (l_full == true)
		{
			m_wnd.create({ g::WW,g::WH,32U }, g::GNAME, sf::Style::Fullscreen);
		}
		else
		{
			m_wnd.create({ g::WW,g::WH,32U }, g::GNAME, sf::Style::Close);
		}
		m_full = l_full;
	}
}

bool Wndw::IsFullscreen()
{
	return m_full;
}

sf::RenderWindow& Wndw::operator()()
{
	return m_wnd;
}
