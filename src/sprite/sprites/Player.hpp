#include <sprite/ASprite.hpp>
#include <action/ActionTarget.hpp>
#include <FSM/FSM_Player.hpp>

#ifndef PLAYER_HPP__
#define PLAYER_HPP__

struct Player : ASprite, ActionTarget<int>
{
	Player() = delete;
	Player(sf::View& l_worldSpace);
	~Player() override = default;

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;


	void processInput() override;
	void update(const sf::Time& l_dt) override;
	static float JumpForce;
	static float MoveSpeed;
	bool IsMoving();
	bool IsShooting() override;
	void Shoot() override;
	void TakeHit(int l_damage);
	void SetMoving(bool l_moving, bool l_right);
	AnimType SyncFSM();
	sf::View& GetView();
	FSM_Player fsm{};
	float prevVelY_{ 0.f };
private:
	void bindActions();
	bool jumpHeld_{ false };
	bool jumpLetGo_{ true };
	bool pressingRight_{ false };
	bool pressingLeft_{ false };
	bool pressingShoot_{ false };

	sf::View& worldView_;



};

#endif