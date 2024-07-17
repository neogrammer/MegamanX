#ifndef BUZZBIRD_HPP__
#define BUZZBIRD_HPP__

#include <sprite/ASprite.hpp>
//#include <action/ActionTarget.hpp>
//#include <FSM/FSM_Player.hpp>


struct BuzzBird : ASprite //, ActionTarget<int>
{
	BuzzBird();
	~BuzzBird() override = default;

	BuzzBird(const BuzzBird&);
	BuzzBird& operator=(const BuzzBird&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;
	//static float JumpForce;
	//static float MoveSpeed;
	//bool IsMoving();
	bool IsShooting();
	void TakeHit(int l_damage);
	void Shoot();

	//AnimType SyncFSM();

	//FSM_Player fsm{};
private:
	int m_hp{ 13 };
	bool m_isShooting{ false };
	sf::Clock m_shootTimer{};
	//void bindActions();
	//bool jumpHeld_{ false };
	//bool jumpLetGo_{ true };


};

#endif