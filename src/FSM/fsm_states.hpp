#ifndef FSM_STATES_HPP__
#define FSM_STATES_HPP__

#include <variant>
#include <misc/globals.hpp>
struct state_Standing { };
struct state_Running { };
struct state_Rising { };
struct state_Falling { };
struct state_ShootingWhileStanding { };
struct state_ShootingWhileRunning {  };
struct state_ShootingWhileRising { };
struct state_ShootingWhileFalling { };
struct state_TransToRun {};

using PlayerStateVar = std::variant<state_Standing, state_Running, state_Rising, state_Falling
								, state_ShootingWhileStanding, state_ShootingWhileRunning
								, state_ShootingWhileRising, state_ShootingWhileFalling, state_TransToRun>;

#endif