#ifndef SANDBOXSTATE_HPP__
#define SANDBOXSTATE_HPP__

#include <game_state/GameState.hpp>
#include <memory>
struct Stage;

struct SandboxState : GameState
{
	explicit SandboxState();
	~SandboxState() override final = default;

	SandboxState(const SandboxState&) = delete;
	SandboxState& operator=(const SandboxState&) = delete;
	SandboxState(SandboxState&&) = delete;
	SandboxState& operator=(SandboxState&&) = delete;

	void Enter() override final;
	void Leave() override final;

	void ProcessInput() override final;
	void HandleEvent(const sf::Event& l_e) override final;
	void Update(const sf::Time& l_dt) override final;
	void Render(sf::RenderWindow& l_wnd) override final;
private:
	std::unique_ptr<Stage> m_stage;

};

#endif