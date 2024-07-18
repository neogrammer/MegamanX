#include <sprite/ASprite.hpp>
#include <action/ActionTarget.hpp>
#include <FSM/FSM_Player.hpp>

#ifndef PLAYER_HPP__
#define PLAYER_HPP__

struct Player : ASprite, ActionTarget<int>
{
	Player();
	~Player() override = default;

	Player(const Player&);
	Player& operator=(const Player&);

	void processInput() override;
	void update(const sf::Time& l_dt) override;
	static float JumpForce;
	static float MoveSpeed;
	bool IsMoving();
	bool IsShooting() override;
	void Shoot() override;
	void TakeHit(int l_damage);

	AnimType SyncFSM();

	FSM_Player fsm{};
private:
	void bindActions();
	bool jumpHeld_{ false };
	bool jumpLetGo_{ true };
	bool pressingRight_{ false };
	bool pressingLeft_{ false };
	bool pressingShoot_{ false };



};

#endif