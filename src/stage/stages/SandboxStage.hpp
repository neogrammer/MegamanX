#ifndef SANDBOXSTAGE_HPP__
#define SANDBOXSTAGE_HPP__

#include <stage/Stage.hpp>

struct SandboxStage : Stage
{
	explicit SandboxStage();
	~SandboxStage() override final = default;

	SandboxStage(const SandboxStage&) = delete;
	SandboxStage& operator=(const SandboxStage&) = delete;
	SandboxStage(SandboxStage&&) = delete;
	SandboxStage& operator=(SandboxStage&&) = delete;

	void ProcessInput() override final;
	void HandleEvent(const sf::Event& l_e) override final;
	void Update(const sf::Time& l_dt) override final;
	void Render(sf::RenderWindow& l_wnd) override final;
private:

};

#endif
