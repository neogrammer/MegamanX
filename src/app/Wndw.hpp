#ifndef WNDW_HPP__
#define WNDW_HPP__
#include <SFML/Graphics/RenderWindow.hpp>

struct Wndw
{
	Wndw();
	~Wndw() = default;

	Wndw(const Wndw&) = delete;
	Wndw& operator=(const Wndw&) = delete;
	Wndw(Wndw&&) = delete;
	Wndw& operator=(Wndw&&) = delete;


	void Fullscreen(bool l_full);
	bool IsFullscreen();
	sf::RenderWindow& operator()();
private:
	sf::RenderWindow m_wnd;
	bool m_full{ false };
};

#endif