#ifndef SANDBOXSTAGE_HPP__
#define SANDBOXSTAGE_HPP__

#include <stage/Stage.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
struct SandboxStage : Stage
{
	explicit SandboxStage();
	~SandboxStage() override final;

	SandboxStage(const SandboxStage&) = delete;
	SandboxStage& operator=(const SandboxStage&) = delete;
	SandboxStage(SandboxStage&&) = delete;
	SandboxStage& operator=(SandboxStage&&) = delete;

	void ProcessInput() override final;
	void HandleEvent(const sf::Event& l_e) override final;
	void Update(const sf::Time& l_dt) override final;
	void Render(sf::RenderWindow& l_wnd) override final;

	sf::Vector2i mpos{0,0};
	sf::Vector2f startPointLine{ 0.f,0.f };
	sf::Vector2f lineDir{ 0.f,0.f };
	sf::Color lineCol{ sf::Color::Yellow };
private:
	sf::RectangleShape boundingBox_;
	sf::CircleShape pointOfContact_;
	sf::VertexArray normLine{ sf::VertexArray(sf::PrimitiveType::Lines, 2) };
	sf::VertexArray castline{sf::VertexArray( sf::PrimitiveType::Lines, 2) };

	bool rayColliding{ false };
	bool playerColliding{ false };

	
	float bgOffset{ 0.f };
};

#endif
