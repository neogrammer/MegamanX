#include "Game.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Color.hpp>
#include <misc/globals.hpp>
#include <SFML/Window/Event.hpp>


void Game::Run()
{
	sf::Time dt{ sf::Time::Zero };
	sf::Clock clock;

	while (m_wnd().isOpen())
	{
		input();

		dt = clock.restart();
		update(dt);
		/*bool repaint = false;
		while (dt >= g::FPS60)
		{
			update(g::FPS60);
			dt -= g::FPS60;
			repaint = true;
		}
		

		if (repaint)*/
			render();
	}

}

void Game::input()
{
	m_stateMgr.ProcessInput();
	
	sf::Event e;
	while (m_wnd().pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			m_wnd().close();
			break;
		case sf::Event::KeyReleased:
			if (e.key.code == sf::Keyboard::Escape)
				m_wnd().close();
			else if (e.key.code == sf::Keyboard::F5)
				m_wnd.Fullscreen(!m_wnd.IsFullscreen());
			break;
		default:
			break;
		}
		m_stateMgr.HandleEvent(e);
	}

	
}
void Game::update(sf::Time l_dt)
{
	m_stateMgr.Update(l_dt);
}

void Game::render()
{
	m_wnd().clear(sf::Color::Blue);

	m_stateMgr.Render(m_wnd());

	m_wnd().display();
}