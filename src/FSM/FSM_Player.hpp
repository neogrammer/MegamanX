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
		return state_Rising{};
	}

	std::optional<PlayerStateVar> On_Event(state_Running& s, const evt_Jumped& e)
	{
		return state_Rising{};
	}

	std::optional<PlayerStateVar> On_Event(state_Rising& s, const evt_ReachedJumpPeak& e)
	{
		return state_Falling{};
	}

	std::optional<PlayerStateVar> On_Event(state_Falling& s, const evt_Landed& e)
	{
		
		if (moving)
		{
			return state_Running{};
		}
		if (!moving)
		{
			return state_Standing{};
		}
	}

	std::optional<PlayerStateVar> On_Event(state_Standing& s, const evt_Fell& e)
	{
		return state_Falling{};
	}

	std::optional<PlayerStateVar> On_Event(state_Running& s, const evt_Fell& e)
	{
		return state_Falling{};
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
	AnimType getType(state_Rising& s)
	{
		return  AnimType::Jump;
	}
	AnimType getType(state_Falling& s)
	{
		return  AnimType::Jump;
	}
	AnimType getType(state_Running& s)
	{
		return  AnimType::Run;
	}
	AnimType getType(state_TransToRun& s)
	{
		return  AnimType::TransRun;
	}

	template <typename State>
	AnimType getType(State& s)
	{
		std::cout << "animation not added to this state yet! STATE: " << typeid(s).name() << std::endl;
		return AnimType::Count;
	}
};

#endif