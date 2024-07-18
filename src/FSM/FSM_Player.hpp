#ifndef FSM_PLAYER_HPP__
#define FSM_PLAYER_HPP__
#include "FSM.hpp"
#include "fsm_states.hpp"
#include "fsm_events.hpp"
#include <optional>
#include <misc/globals.hpp>
#include <iostream>
#include <type_traits>
class FSM_Player : public FSM<FSM_Player, PlayerStateVar>
{

public:
	// state flags set from outside
	bool moving{};

	std::optional<PlayerStateVar> On_Event(state_TransToRun& s, const evt_Jumped& e)
	{
		return state_TransToJump{};
	}

	std::optional<PlayerStateVar> On_Event(state_TransToJump& s, const evt_TransToRiseCompleted& e)
	{
		return state_Rising{};
	}

	std::optional<PlayerStateVar> On_Event(state_TransToJump& s, const evt_ReachedJumpPeak& e)
	{
		return state_TransToFall{};
	}

	//std::optional<PlayerStateVar> On_Event(state_TransToRun& s, const evt_Fell& e)
	//{
	//	return state_Falling{};
	//}

	std::optional<PlayerStateVar> On_Event(state_Rising& s, const evt_ReachedJumpPeak& e)
	{
		return state_TransToFall{};
	}

	std::optional<PlayerStateVar> On_Event(state_TransToFall& s, const evt_TransToFallingCompleted& e)
	{
		return state_Falling{};
	}

	std::optional<PlayerStateVar> On_Event(state_TransToFall& s, const evt_Landed& e)
	{
		return state_Landing{};
	}

	std::optional<PlayerStateVar> On_Event(state_Falling& s, const evt_Landed& e)
	{
		return state_Landing{};
	}
	std::optional<PlayerStateVar> On_Event(state_Landing& s, const evt_Jumped& e)
	{
		return state_TransToJump{};
	}
	std::optional<PlayerStateVar> On_Event(state_Landing& s, const evt_StartedMoving& e)
	{
		return state_TransToRun{};
	}
	std::optional<PlayerStateVar> On_Event(state_Landing& s, const evt_Landed& e)
	{
		return state_Standing{};
	}

	std::optional<PlayerStateVar> On_Event(state_Standing& s, const evt_StartedShooting& e)
	{
		return state_ShootingWhileStanding{};
	}

	std::optional<PlayerStateVar> On_Event(state_ShootingWhileStanding& s, const evt_StoppedShooting& e)
	{
		return state_Standing{};
	}

	std::optional<PlayerStateVar> On_Event(state_ShootingWhileStanding& s, const evt_Jumped& e)
	{
		return state_TransToJump{};
	}
	std::optional<PlayerStateVar> On_Event(state_Standing& s, const evt_StartedMoving& e)
	{
		return state_TransToRun{};
	}
	std::optional<PlayerStateVar> On_Event(state_TransToRun& s, const evt_StartedMoving& e)
	{
		return state_Running{};
	}
	std::optional<PlayerStateVar> On_Event(state_TransToRun& s, const evt_StoppedMoving& e)
	{
		return state_Standing{};
	}

	std::optional<PlayerStateVar> On_Event(state_Running& s, const evt_StoppedMoving& e)
	{
		return state_Standing{};
	}

	std::optional<PlayerStateVar> On_Event(state_Standing& s, const evt_Jumped& e)
	{
		return state_TransToJump{};
	}

	std::optional<PlayerStateVar> On_Event(state_Running& s, const evt_Jumped& e)
	{
		return state_TransToJump{};
	}

	template <typename State, typename Event>
	std::optional<PlayerStateVar> On_Event(State&, const Event&)
	{
		return std::nullopt;
	}



	AnimType getType(state_Standing& s)
	{
		return AnimType::Idle;
	}

	AnimType getType(state_Running& s)
	{
		return  AnimType::Run;
	}
	AnimType getType(state_TransToRun& s)
	{
		return  AnimType::TransRun;
	}
	AnimType getType(state_ShootingWhileStanding& s)
	{
		return AnimType::ShootStand;
	}
	AnimType getType(state_Rising& s)
	{
		return  AnimType::Rise;
	}
	AnimType getType(state_Falling& s)
	{
		return  AnimType::Fall;
	}
	AnimType getType(state_Landing& s)
	{
		return  AnimType::Land;
	}
	AnimType getType(state_TransToJump& s)
	{
		return  AnimType::TransJump;
	}
	AnimType getType(state_TransToFall& s)
	{
		return  AnimType::TransFall;
	}


	template <typename State>
	AnimType getType(State& s)
	{
		std::cout << "animation not added to this state yet! STATE: " << typeid(s).name() << std::endl;
		return AnimType::Count;
	}
};

#endif