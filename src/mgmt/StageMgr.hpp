#ifndef STAGEMGR_HPP__
#define STAGEMGR_HPP__

#include <unordered_map>
#include <stack>
#include <memory>

#include <stage/Stage.hpp>

struct StageMgr
{
	StageMgr() = delete;
	explicit StageMgr(StageType l_stage);
	~StageMgr() = default;

	StageMgr(const StageMgr&) = delete;
	StageMgr& operator=(const StageMgr&) = delete;
	StageMgr(StageMgr&&) = delete;
	StageMgr& operator=(StageMgr&&) = delete;

	void ProcessInput();
	void HandleEvent(const sf::Event& l_e);
	void Update(const sf::Time& l_dt);
	void Render(sf::RenderWindow& l_wnd);

private:
	std::unique_ptr<Stage> m_currStage;
};

#endif