#include "StageMgr.hpp"
#include <stage/stages/SandboxStage.hpp>
StageMgr::StageMgr(StageType l_stage)
	: m_currStage{}
{
	switch (l_stage)
	{
	case StageType::Sandbox:
	{
		m_currStage = std::make_unique<SandboxStage>();
	}
		break;
	default:
		break;
	}


}

void StageMgr::ProcessInput()
{
	m_currStage->ProcessInput();
}

void StageMgr::HandleEvent(const sf::Event& l_e)
{
	m_currStage->HandleEvent(l_e);
}

void StageMgr::Update(const sf::Time& l_dt)
{
	m_currStage->Update(l_dt);
}

void StageMgr::Render(sf::RenderWindow& l_wnd)
{
	m_currStage->Render(l_wnd);
}
