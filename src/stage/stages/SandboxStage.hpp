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
private:
	sf::RectangleShape boundingBox_;
	sf::CircleShape pointOfContact_;

	float bgOffset{ 0.f };
};

#endif
