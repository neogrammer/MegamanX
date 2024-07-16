#ifndef FSM_PLAYER_HPP__
#define FSM_PLAYER_HPP__
#include "FSM.hpp"
#include "fsm_states.hpp"
#include "fsm_events.hpp"
#include <optional>
#include <misc/globals.hpp>
class FSM_Player : public FSM<FSM_Player, PlayerStateVar>
{

public:
	// state flags set from outside
	bool moving{};

	std::optional<PlayerStateVar> On_Event(state_Standing& s, const evt_StartedMoving& e)
	{
		return state_Running{};
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
		return s.getType();
	}
	AnimType getType(state_Rising& s)
	{
		return s.getType();
	}
	AnimType getType(state_Falling& s)
	{
		return s.getType();
	}
	AnimType getType(state_Running& s)
	{
		return s.getType();
	}
};

#endif